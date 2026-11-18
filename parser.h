#ifndef HEADIFY_PARSER_H
#define HEADIFY_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADIFY_VAR "@var"
#define HEADIFY_FUNCTION "@function"
#define HEADIFY_COMMENT "@comment"
#define HEADIFY_STRUCT "@struct"
#define HEADIFY_DEFINE "@define"

static char *headify_trim(char *text)
{
    char *end;

    while (isspace((unsigned char)*text))
        text++;
    if (*text == '\0')
        return text;

    end = text + strlen(text) - 1;
    while (end > text && isspace((unsigned char)*end))
        end--;
    end[1] = '\0';
    return text;
}

static int headify_starts_with(const char *text, const char *prefix)
{
    return strncmp(text, prefix, strlen(prefix)) == 0;
}

static char *headify_skip_space(char *text)
{
    while (isspace((unsigned char)*text))
        text++;
    return text;
}

static int headify_brace_delta(const char *text)
{
    int delta = 0;
    while (*text) {
        if (*text == '{') delta++;
        else if (*text == '}') delta--;
        text++;
    }
    return delta;
}

static void headify_guard_name(const char *filename, char *guard, size_t size)
{
    const char *base = strrchr(filename, '/');
    size_t used = 0;

    base = base ? base + 1 : filename;
    while (*base && used + 4 < size) {
        unsigned char ch = (unsigned char)*base++;
        if (isalnum(ch))
            guard[used++] = (char)toupper(ch);
        else if (used == 0 || guard[used - 1] != '_')
            guard[used++] = '_';
    }
    while (used > 0 && guard[used - 1] == '_')
        used--;
    if (used == 0 && size > 1)
        guard[used++] = 'H';
    if (!(guard[0] >= 'A' && guard[0] <= 'Z')) {
        memmove(guard + 1, guard, used);
        guard[0] = 'H';
        used++;
    }
    memcpy(guard + used, "_H", 3);
}

static int headify_write_header_line(FILE *header, const char *keyword,
                                     const char *line, int prototype)
{
    char *copy = malloc(strlen(line) + 1);
    char *content;
    int result;

    if (!copy)
        return -1;
    strcpy(copy, line);
    content = headify_trim(copy);
    content += strlen(keyword);
    content = headify_trim(content);
    if (prototype) {
        char *brace = strchr(content, '{');
        if (brace)
            *brace = '\0';
        content = headify_trim(content);
        result = fprintf(header, "%s;\n", content) < 0 ? -1 : 0;
    } else {
        result = fprintf(header, "%s\n", content) < 0 ? -1 : 0;
    }
    free(copy);
    return result;
}

/* Generates the requested header and a cleaned source file. */
static int headify_parse(const char *source_path, const char *header_path)
{
    FILE *source = NULL, *header = NULL, *clean_source = NULL;
    char *line = NULL;
    size_t capacity = 0;
    ssize_t length;
    char *generated_source_path = NULL;
    char *include_name;
    const char *slash;
    char guard[512];
    int in_struct = 0, struct_depth = 0, status = -1;

    source = fopen(source_path, "r");
    if (!source) {
        perror(source_path);
        goto done;
    }
    header = fopen(header_path, "w");
    if (!header) {
        perror(header_path);
        goto done;
    }

    generated_source_path = malloc(strlen(source_path) + sizeof("-generated.c"));
    if (!generated_source_path) {
        perror("headify: memory allocation");
        goto done;
    }
    sprintf(generated_source_path, "%s-generated.c", source_path);
    clean_source = fopen(generated_source_path, "w");
    if (!clean_source) {
        perror(generated_source_path);
        goto done;
    }

    slash = strrchr(header_path, '/');
    include_name = (char *)(slash ? slash + 1 : header_path);
    headify_guard_name(include_name, guard, sizeof(guard));
    if (fprintf(header, "#ifndef %s\n#define %s\n\n", guard, guard) < 0 ||
        fprintf(clean_source, "#include \"%s\"\n", include_name) < 0)
        goto write_error;

    while ((length = getline(&line, &capacity, source)) != -1) {
        char *content = headify_skip_space(line);
        int omit_from_source = 0;

        if (in_struct) {
            if (fputs(line, header) == EOF)
                goto write_error;
            struct_depth += headify_brace_delta(line);
            if (struct_depth <= 0 && strchr(line, '}')) in_struct = 0;
            omit_from_source = 1;
        } else if (headify_starts_with(content, HEADIFY_VAR)) {
            if (headify_write_header_line(header, HEADIFY_VAR, content, 0) < 0)
                goto write_error;
            omit_from_source = 1;
        } else if (headify_starts_with(content, HEADIFY_FUNCTION)) {
            char *function = content + strlen(HEADIFY_FUNCTION);
            char *brace;
            if (headify_write_header_line(header, HEADIFY_FUNCTION, content, 1) < 0)
                goto write_error;
            function = headify_trim(function);
            brace = strchr(function, '{');
            if (brace) {
                int function_depth = headify_brace_delta(brace);
                /* Keep an inline function body intact in the generated source. */
                if (fputs(function, clean_source) == EOF)
                    goto write_error;
                if (length > 0 && line[length - 1] == '\n' && fputc('\n', clean_source) == EOF)
                    goto write_error;
                while (function_depth > 0) {
                    while ((length = getline(&line, &capacity, source)) != -1) {
                        if (fputs(line, clean_source) == EOF)
                            goto write_error;
                        function_depth += headify_brace_delta(line);
                        if (function_depth <= 0) break;
                    }
                }
                omit_from_source = 1;
            } else {
                if (fprintf(clean_source, "%s\n", function) < 0)
                    goto write_error;
            }
        } else if (headify_starts_with(content, HEADIFY_COMMENT)) {
            if (headify_write_header_line(header, HEADIFY_COMMENT, content, 0) < 0)
                goto write_error;
            omit_from_source = 1;
        } else if (headify_starts_with(content, HEADIFY_STRUCT)) {
            char *definition = headify_trim(content + strlen(HEADIFY_STRUCT));
            if (fprintf(header, "%s\n", definition) < 0)
                goto write_error;
            in_struct = 1;
            struct_depth = headify_brace_delta(definition);
            if (struct_depth <= 0 && strchr(definition, '}')) in_struct = 0;
            omit_from_source = 1;
        } else if (headify_starts_with(content, HEADIFY_DEFINE)) {
            char *definition = headify_trim(content + strlen(HEADIFY_DEFINE));
            if (fprintf(header, "#define %s", definition) < 0)
                goto write_error;
            if (!strchr(definition, '\n') && length > 0 && line[length - 1] == '\n')
                if (fputc('\n', header) == EOF) goto write_error;
            omit_from_source = 1;
        }

        if (!omit_from_source && fputs(line, clean_source) == EOF)
            goto write_error;
    }
    if (ferror(source))
        goto write_error;
    if (fprintf(header, "\n#endif /* %s */\n", guard) < 0)
        goto write_error;
    status = 0;
    goto done;

write_error:
    perror("headify: writing generated files");
done:
    free(line);
    free(generated_source_path);
    if (clean_source && fclose(clean_source) != 0) status = -1;
    if (header && fclose(header) != 0) status = -1;
    if (source) fclose(source);
    return status;
}

#endif
