#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "utils.c"


void parse(FILE *input_loaded){
    char *line = NULL;
    size_t tam = 0;
    char aux[1024] = {0}; 

    while (getline(&line, &tam, input_loaded) != -1) {

        // To Global Vars
        if (strncmp(line, "@var_global", 11) == 0) {

            strcpy(aux, line);
            remove_char_if_exists(aux, '\n');
            remove_substring(aux, "@var_global");
            remove_char_if_exists(aux, '{');

            char *trimmed = trim(aux);
            strcpy(aux, trimmed);
            // Output 
            puts(aux);
            continue;
        }

        // Function state
        if (strncmp(line, "@function", 9) == 0) {

            strcpy(aux, line);
            remove_char_if_exists(aux, '\n');
            remove_substring(aux, "@function");
            remove_char_if_exists(aux, '{');
            char *trimmed = trim(aux);
            strcpy(aux, trimmed);
            // Output 
            puts(aux);
            continue;
        }

    }

    free(line);
}

#endif