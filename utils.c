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

void remove_char(char *str, char ch) {

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

#endif
