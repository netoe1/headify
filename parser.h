#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "utils.h"

#define F0_GLOBAL_VARIABLE "@variable"
#define F1_FUNCTION "@function"
#define F2_COMMENT "@comment"

void parse(FILE *input_loaded, char *input_filename, FILE *output_file, char *output_filename){

    char *line = NULL;
    size_t tam = 0;
    char aux[1024] = {0};

    // Copy filename to modify
    char input_filename_mod[strlen(input_filename) + 1];
    strcpy(input_filename_mod, input_filename);

    // Trim filename
    char *trimmed_filename = trim(input_filename_mod);
    remove_substring(trimmed_filename,".c");
    to_upper_string(trimmed_filename); 

    // Open output file
    output_file = fopen(output_filename,"w");

    if(!output_file){
        perror("headify-err-parse(): Cannot open file .h! Aborting...");
        goto end;
    }
    
    // Header guards
    fprintf(output_file,"#ifndef %s_H\n#define %s_H\n",
            trimmed_filename, trimmed_filename);

    // Parse loop
    while (getline(&line, &tam, input_loaded) != -1) {  

        // ---------------- @variable ----------------
        if (starts_with(F0_GLOBAL_VARIABLE, line) == 0) {

            strncpy(aux, line, sizeof(aux)-1);
            aux[sizeof(aux)-1] = '\0';

            remove_char_if_exists(aux, '\n');
            remove_substring(aux, F0_GLOBAL_VARIABLE);

            char *trimmed_aux = trim(aux);

            puts("headify-debug-@variable:");
            puts(trimmed_aux);

            fprintf(output_file,"%s\n",trimmed_aux);
            continue;
        }

        // ---------------- @function ----------------
        if (starts_with(F1_FUNCTION,line) == 0) {

            strncpy(aux, line, sizeof(aux)-1);
            aux[sizeof(aux)-1] = '\0';

            remove_char_if_exists(aux, '\n');
            remove_substring(aux, F1_FUNCTION);
            remove_char_if_exists(aux, '{');

            char *trimmed_aux = trim(aux);

            puts("headify-debug-@function:");
            puts(trimmed_aux);

            fprintf(output_file,"%s;\n",trimmed_aux);
            continue;
        }

        // ---------------- @comment ----------------
        if (starts_with(F2_COMMENT,line) == 0) {

            strncpy(aux, line, sizeof(aux)-1);
            aux[sizeof(aux)-1] = '\0';

            remove_char_if_exists(aux, '\n');
            remove_substring(aux, F2_COMMENT);
            remove_char_if_exists(aux, '{');

            char *trimmed_aux = trim(aux);

            puts("headify-debug-@comment:");
            puts(trimmed_aux);

            fprintf(output_file,"%s\n",trimmed_aux);
            continue;
        }
    }
    
    fprintf(output_file,"#endif\n");


    // ---------------------------------------------
    // SECOND LOOP - CLEANING FLAGS FROM .c FILE
    // ---------------------------------------------

    rewind(input_loaded);
    char input_mod[256];

    snprintf(input_mod, sizeof(input_mod), "%s-mod.c", input_filename);

    FILE *clean_c = fopen(input_mod,"w");

    if (!clean_c) {
        perror("Cannot open cleaned .c file");
        goto end;
    }

    // Include .h in source
    fprintf(clean_c,"#include \"%s\"\n",output_filename);

    while (getline(&line, &tam, input_loaded) != -1) {

        if (starts_with(F0_GLOBAL_VARIABLE, line) == 0) {
            continue;
        }

        if (starts_with(F1_FUNCTION, line) == 0) {

            char *content = trim(line + strlen(F1_FUNCTION));
            fprintf(clean_c, "%s\n", content);
            continue;
        }

        if (starts_with(F2_COMMENT, line) == 0) {

            char *content = trim(line + strlen(F2_COMMENT));
            fprintf(clean_c, "%s\n", content);
            continue;
        }

        // linhas normais
        fprintf(clean_c, "%s", line);
    }

    fclose(clean_c);

end:

    free(line);

    if(output_file)
        fclose(output_file);
}

#endif
