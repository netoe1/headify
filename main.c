#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static void show_help(const char *program)
{
    printf("Usage:\n"
           "  %s --help | -h\n"
           "  %s --LICENSE\n"
           "  %s generate <input.headify> <output.h>\n\n"
           "Reads annotations in a source file and creates the requested header\n"
           "plus an <input.headify>-generated.c file with exported annotations removed.\n\n"
           "Annotations: @var, @function, @comment, @struct, @define\n",
           program, program, program);
}

static void show_license(void)
{
    puts("MIT License\n\n"
         "Copyright (c) 2026 Ely Torres Neto / Vertex Project\n\n"
         "Permission is hereby granted, free of charge, to any person obtaining a copy\n"
         "of this software and associated documentation files (the \"Software\"), to deal\n"
         "in the Software without restriction, including without limitation the rights\n"
         "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
         "copies of the Software, and to permit persons to whom the Software is\n"
         "furnished to do so, subject to the following conditions:\n\n"
         "The above copyright notice and this permission notice shall be included in all\n"
         "copies or substantial portions of the Software.\n\n"
         "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
         "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
         "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
         "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
         "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
         "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
         "SOFTWARE.");
}

static int has_suffix(const char *filename, const char *suffix)
{
    size_t filename_length = strlen(filename);
    size_t suffix_length = strlen(suffix);
    return filename_length >= suffix_length &&
           strcmp(filename + filename_length - suffix_length, suffix) == 0;
}

int main(int argc, char **argv)
{
    if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)) {
        show_help(argv[0]);
        return EXIT_SUCCESS;
    }
    if (argc == 2 && strcmp(argv[1], "--LICENSE") == 0) {
        show_license();
        return EXIT_SUCCESS;
    }
    if (argc != 4 || strcmp(argv[1], "generate") != 0) {
        show_help(argv[0]);
        return EXIT_FAILURE;
    }
    if (!has_suffix(argv[2], ".headify")) {
        fprintf(stderr, "headify: input file must end with .headify: %s\n", argv[2]);
        return EXIT_FAILURE;
    }
    if (!has_suffix(argv[3], ".h")) {
        fprintf(stderr, "headify: output file must end with .h: %s\n", argv[3]);
        return EXIT_FAILURE;
    }

    return headify_parse(argv[2], argv[3]) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
