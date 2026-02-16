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

void parse(FILE *input_loaded,char *input_filename,FILE *output_file,char *output_filename){

    char *line = NULL;          //  line buffer to use getline()
    size_t tam = 0;             //  tam as size for line buffer.  
    char aux[1024] = {0}; 

    // Copy filename to modify

    char input_filename_mod[strlen(input_filename) + 1];
    strcpy(input_filename_mod, input_filename);

    // Trim usually "jumps" addresses, this is the reason we should use trimmed instead of input_filename_mod

    char *trimmed = trim(input_filename_mod);
    remove_substring(trimmed,".c");
    to_upper_string(trimmed); 

    // Open outputfile and write #ifndef/#define, being the same name written on .c file 

    output_file = fopen(output_filename,"w");

    // In case of error while opening file
    if(!output_file){
        perror("headify-err-parse(): Cannot open file .h! Aborting...");
        goto end;
    }
    
    // Writing #ifndef/#define on header file
    fprintf(output_file,"#ifndef %s_H\n#define %s_H\n",trimmed,trimmed);

    // Starting parse process
    while (getline(&line, &tam, input_loaded) != -1) {  

        // To Global Vars
        if (strncmp(line, "@variable", 9) == 0) {

            strcpy(aux, line);
            remove_char_if_exists(aux, '\n');
            remove_substring(aux, "@variable");
            remove_char_if_exists(aux, '{');

            char *trimmed = trim(aux);
            strcpy(aux, trimmed);
            puts("headify-debug-@variable:");
            puts(aux);
            fprintf(output_file,"%s\n",aux);
            continue;
        }

        // Testing function name
        
        if (strncmp(line, "@function", 9) == 0) {

            strcpy(aux, line);
            remove_char_if_exists(aux, '\n');
            remove_substring(aux, "@function");
            remove_char_if_exists(aux, '{');

            char *trimmed = trim(aux);
            strcpy(aux, trimmed);
            fprintf(output_file,"%s;\n",aux);

            // Output 
            puts("headify-debug-@function:");
            puts(aux);
            continue;
        }

        // Comments that goes to header

        if (strncmp(line, "@comment", 8) == 0) {

            strcpy(aux, line);
            remove_char_if_exists(aux, '\n');
            remove_substring(aux, "@comment");
            remove_char_if_exists(aux, '{');

            char *trimmed = trim(aux);
            strcpy(aux, trimmed);
            fprintf(output_file,"%s\n",aux);

            // Output 
            puts("headify-debug-@comment:");
            puts(aux);
            continue;
        }

    }
    
    fprintf(output_file,"#endif");


    end:
    // Free lines and closing output file.
    // Input file will be closed on int main() from main.c
    free(line);
    fclose(output_file);
}

#endif