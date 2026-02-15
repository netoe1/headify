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
#endif
