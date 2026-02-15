/*
MIT License

Copyright (c) 2026 Ely Torres Neto

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef UTILS_C
#define UTILS_C
#include <ctype.h>

char *trim(char *str)
{
    char *end;

    // Trim do início (remover espaços à esquerda)
    while (isspace((unsigned char)*str))
    {
        str++;
    }

    // Se a string for só de espaços, retorna uma string vazia
    if (*str == '\0')
    {
        return str;
    }

    // Trim do final (remover espaços à direita)
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
    {
        end--;
    }

    // Coloca o caractere nulo após o último caractere não-espaço
    *(end + 1) = '\0';

    return str;
}

void to_lower_string(char str[]){
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int empty_line(char *s) {
    while (*s) {
        if (!isspace((unsigned char)*s))
            return 0; 
        s++;
    }
    return 1; 
}

void remove_substring(char *str, const char *remove) {

    char *pos = strstr(str, remove);

    if (pos) {
        memmove(pos,
                pos + strlen(remove),
                strlen(pos + strlen(remove)) + 1);
    }
}

void remove_char_if_exists(char *str, char ch) {

    char *src = str;
    char *dst = str;

    while (*src) {

        if (*src != ch) {
            *dst = *src;
            dst++;
        }

        src++;
    }

    *dst = '\0';
}

void remove_quote(char *str){

    //printf("\nBefore quote: %s",str);

    for(int i = 0; i < sizeof(str);i++){
        if(str[i] == '"'){
            str[i] = ' ';
        }
    }

    str = trim(str);
    //printf("\nAfter quote:%s",str);
}

void trim_all_args(int argc,char *argv[]){
    int i = 0;
    for(i = 1;i < argc;i++){
         argv[i] = trim(argv[i]);
    }
}
#endif
