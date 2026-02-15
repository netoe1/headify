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
#include "utils.c"

#define TTHELP 0
#define THELP 1

#define MATCH 0

#define or ||
#define and &&

#define GENERATE_ARGV argv[1]
#define GENERATE_STR "generate"
#define SRC_C_ARGV argv[2]
#define OUT_H_ARGV argv[3]
#define HELP_ARGV argv[1]


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


const char *CONSTS_ARGC2[] = {"--help","-h"};


void trim_all_args(int argc,char *argv[]){
    int i = 0;
    for(i = 1;i < argc;i++){
         argv[i] = trim(argv[i]);
    }
}

void remove_quote(char *str){

    printf("\nBefore quote: %s",str);

    for(int i = 0; i < sizeof(str);i++){
        if(str[i] == '"'){
            str[i] = ' ';
        }
    }

    str = trim(str);
    printf("\nAfter quote:%s",str);
}

void show_help(){
    puts("Entrou no help!");
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
            if(strcmp(CONSTS_ARGC2[TTHELP],HELP_ARGV) == MATCH or strcmp(CONSTS_ARGC2[THELP],HELP_ARGV) == MATCH){

                show_help();

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
                    puts("headify-err: not recognized file with .c at <src_c> field.");
                    goto err;
                }

                if(strstr(OUT_H_ARGV,".h") == NULL){
                    puts("headify-err: not recognized file with .h at <out.h> field.");
                    goto err;
                }

                // Removing quotes from path's
                remove_quote(OUT_H_ARGV);
                remove_quote(SRC_C_ARGV);

                // Open .c file
                c_file = fopen(c_file,"r");
        

                if(!c_file){
                    perror("headify-err: the .c file doesn't exist or doesn't have enough permissions to read.");
                    goto end;
                }

                // if(!h_file){
                //      perror("headify-err: the .h file doesn't exist or doesn't have enough permissions to read.");
                //      goto end;
                // }



                

                goto end;


            }

            puts("headify-err:(c4) command not recognized! Check --help!");
            goto err;
        }

    }

    err:
    return EXIT_FAILURE;

    end:
    return 0;
}

