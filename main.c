/*
 * MIT License
 *
 * Copyright (c) 2026 Ely Torres Neto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <errno.h>
#include "utils.h"
#include "parser.h"



#define or ||
#define and &&

// Alias
#define LICENSE_ARGV argv[1]
#define GENERATE_ARGV argv[1]
#define SRC_C_ARGV argv[2]
#define OUT_H_ARGV argv[3]
#define HELP_ARGV argv[1]
#define MATCH 0

// STR
#define HELP_STR1 "--help"
#define HELP_STR2 "-h"
#define LICENSE_STR "--LICENSE"
#define GENERATE_STR "generate"

/*
    DEFINING BEHAVIOR:

    ARGC = 3
        - headify generate <src.c> <out.h>
    ARGC = 2
        - headify --help or -h
    ARGC = 1
        - headify
*/


// Global Variables

FILE *c_file = NULL;
FILE *h_file = NULL;

const char *help_message =  
"headify - just drag your .c file!\n"
"autor: Ely Torres Neto (netoe1)\tgithub.com/netoe1\n"
"==>HELP==============================================================================\n"
"   --help or -h                          Show valid lines for usage!\n"
"   --LICENSE                             Display the license, this is currently MIT.\n"
"   headify generate <src.c> <out.h>      Generates the header file by src.c\n"
;

const char *LICENSE =
"\n"
"MIT License\n"
"\n"
"Copyright (c) 2026 Ely Torres Neto\n"
"\n"
"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
"of this software and associated documentation files (the \"Software\"), to deal\n"
"in the Software without restriction, including without limitation the rights\n"
"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell\n"
"copies of the Software, and to permit persons to whom the Software is\n"
"furnished to do so, subject to the following conditions:\n"
"\n"
"The above copyright notice and this permission notice shall be included in all\n"
"copies or substantial portions of the Software.\n"
"\n"
"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\n"
"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
"SOFTWARE.\n"
"\n"
"\n";



void show_help(){
    puts(help_message);
}

void show_license(){
    puts(LICENSE);
}


int main(int argc, char *argv[]){
    setlocale(LC_ALL,"");

    trim_all_args(argc,argv);

    if(argc > 1 || argc <=3){

        if(argc == 1){

            puts("headify-err(c1): command not recognized! Check --help!");
            return EXIT_FAILURE;
        }

        if(argc == 2){

            // Tratando caso --help ou --h
            if(
                strcmp(HELP_STR1,HELP_ARGV) == MATCH or 
                strcmp(HELP_STR2,HELP_ARGV) == MATCH){

                show_help();

                goto end;
            }
            
            if(strcmp(LICENSE_STR,LICENSE_ARGV) == MATCH){
                show_license();
                goto end;
            }   

            puts("headify-err(c2): command not recognized! Check --help!");
            goto err;
        }

        if(argc == 3){
            puts("headify-err(c3): command not recognized! Check --help!");
            goto err;
        }

        if(argc == 4){

            to_lower_string(GENERATE_ARGV);

            if(strcmp(GENERATE_ARGV,GENERATE_STR) == MATCH){

                // Verifica se o SRC_C possui o .c

                if(strstr(SRC_C_ARGV,".c") == NULL){
                    puts("headify-err:(c4) not recognized file with .c at <src_c> field.");
                    goto err;
                }

                if(strstr(OUT_H_ARGV,".h") == NULL){
                    puts("headify-err:(c4) not recognized file with .h at <out.h> field.");
                    goto err;
                }

                // Removing quotes from path's
                remove_quote(OUT_H_ARGV);
                remove_quote(SRC_C_ARGV);

                // Open .c file
                c_file = fopen(SRC_C_ARGV,"r");
        
                if(!c_file){
                    perror("headify-err:(c4) the .c file doesn't exist or doesn't have enough permissions to read.");
                    goto end;
                }  

                parse(c_file,SRC_C_ARGV,h_file,OUT_H_ARGV);
                goto end;


            }

            puts("headify-err:(c4) command not recognized! Check --help!");
            goto err;
        }

    }





    
    err:
    return EXIT_FAILURE;

    end:

    if(c_file){
        fclose(c_file);
    }
    
    return 0;
}

