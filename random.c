//solução feita com base na página: https://www.vivaolinux.com.br/topico/C-C++/String-Aleatoria
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <random.h>

#define MIN_STRING 5
#define MAX_STRING 20 //
#define MIN_NUMBER 0
#define MAX_NUMBER 10

char *randon_string()
{
    char *validchars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i, strlen;
    char *string;

    srand(time(NULL));

    strlen = MIN_STRING + (rand() % (MAX_STRING - MIN_STRING));
    string = malloc((strlen) * sizeof(char));
    assert(string);

    for (i = 0; i < strlen - 1; i++)
    {
        string[i] = validchars[rand() % 52];
    }
    string[strlen - 1] = '/0';

    return string;
}
int randon_number()
{

    srand(time(NULL));
    return MIN_NUMBER + (rand() % (MAX_NUMBER - MIN_NUMBER));
}