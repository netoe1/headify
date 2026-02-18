#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "utils.h"

#define F0_GLOBAL_VARIABLE "@var"
#define F1_FUNCTION "@function"
#define F2_COMMENT "@comment"
#define F3_STRUCT "@struct"


void parse(FILE *input_loaded, char *input_filename, FILE *output_file, char *output_filename){

    char *line = NULL;
    size_t tam = 0;
    char aux[1024] = {0};

    int reading_struct = 0;     // Flag for reading struct
    int skipping_struct = 0;    // Flag for skip struct 

    // Copy filename to modify
    char input_filename_mod[strlen(input_filename) + 1];
    strcpy(input_filename_mod, input_filename);

    // Trim filename
    char *trimmed_filename = trim(input_filename_mod);
    remove_substring(trimmed_filename,".c");
    to_upper_string(trimmed_filename); 

    // Creating .h name to outputFile generated.
    char temp[256];
    strncpy(temp, output_filename, sizeof(temp)-1);
    temp[sizeof(temp)-1] = '\0';

    char *trimmed_output_file = trim(temp);
    char generated_name[256];

    snprintf(generated_name,
            sizeof(generated_name),
            "%s-generated.h",
            trimmed_output_file);


    // Opening Files
    input_loaded = fopen(input_filename,"r");

    if(!input_loaded){
        perror("headify-err:(c4) the .c file doesn't exist or doesn't have enough permissions to read.");
        goto end;
    }  

    output_file = fopen(generated_name,"w");

    if(!output_file){
        perror("headify-err-parse(): Cannot open file .h! Aborting...");
        goto end;
    }
    
    
    replace_char_if_exists(trimmed_filename,'.','_');
    // Header guards
    fprintf(output_file,"#ifndef %s_H\n#define %s_H\n",
            trimmed_filename, trimmed_filename);

    // Parse loop
    while (getline(&line, &tam, input_loaded) != -1) {  

      
        // States
        if (reading_struct) {
            fprintf(output_file,"%s", line);
        if (strchr(line, '}') != NULL)
            reading_struct = 0;
            continue;
        }

        // ---------------- @variable ----------------
        if (starts_with(F0_GLOBAL_VARIABLE, line) == 0) {

            strncpy(aux, line, sizeof(aux)-1);
            aux[sizeof(aux)-1] = '\0';

            remove_char_if_exists(aux, '\n');
            remove_substring(aux, F0_GLOBAL_VARIABLE);

            char *trimmed_aux = trim(aux);

            //puts("headify-debug-@variable:");
            //puts(trimmed_aux);

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

            //puts("headify-debug-@function:");
            //puts(trimmed_aux);

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

           // puts("headify-debug-@comment:");
           // puts(trimmed_aux);

            fprintf(output_file,"%s\n",trimmed_aux);
            continue;
        }

        if (starts_with("@struct", line) == 0) {
            reading_struct = 1;
            char *content = trim(line + strlen("@struct"));
            fprintf(output_file,"%s\n", content);
            continue;
        }
    }
    
    fprintf(output_file,"#endif\n");


    // ---------------------------------------------
    // SECOND LOOP - CLEANING FLAGS FROM .c FILE
    // ---------------------------------------------

    rewind(input_loaded);
    char input_mod[256];

    snprintf(input_mod, sizeof(input_mod), "%s-generated.c", input_filename);

    FILE *clean_c = fopen(input_mod,"w");

    if (!clean_c) {
        perror("headify-err: cannot open cleaned .c file");
        goto end;
    }

    // Include .h in source
    fprintf(clean_c,"#include \"%s\"\n",generated_name);

    while (getline(&line, &tam, input_loaded) != -1) {
        
        // States
        if (skipping_struct) {
          
            if (strchr(line, '}') != NULL) {
                skipping_struct = 0;
            }
            continue; 
        }

       
        if (starts_with(F0_GLOBAL_VARIABLE, line) == 0) {
            continue;
        }

        if (starts_with(F1_FUNCTION, line) == 0) {

            char *content = trim(line + strlen(F1_FUNCTION));
            fprintf(clean_c, "%s\n", content);
            continue;
        }

        if (starts_with(F2_COMMENT, line) == 0) {
            //char *content = trim(line + strlen(F2_COMMENT));
            //fprintf(clean_c, "%s\n", content);
            continue;
        }

        if (starts_with(F3_STRUCT, line) == 0) {
            skipping_struct = 1;
            continue; 
        }

        fprintf(clean_c, "%s", line);
    }

    fclose(clean_c);

end:

    free(line);

    if(output_file)
        fclose(output_file);
}

#endif
