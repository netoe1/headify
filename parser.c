#ifndef PARSER_C
#define PARSER_C

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "utils.c"

void parse(FILE *input_loaded, FILE *output){

    char *line = NULL;
    size_t tam = 0;
    char aux[1024];

    while (getline(&line, &tam, input_loaded) != -1) {

        if (strncmp(line, "@var_global", 11) == 0) {

            strcpy(aux, line);

            remove_substring(aux, "@var_global");
            remove_char(aux, '{');

            char *trimmed = trim(aux);
            strcpy(aux, trimmed);

            continue;
        }
    }

    free(line);
}

#endif