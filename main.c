
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "datafile.h"
#include "btree.h"
#include "random.h"

#define N_RECORDS 2000 //variavel que define quantos registros serão adicionados na primeira execução
int main(void)
{
    BTREE *tree = start_tree();
    FILE *data = start_datafile;
    if(tree->first_execution) populate(tree,data);
    int switcher = 0;
    while (switcher != 3)
    {
        printf("\nDigite:\n-1 Para adicionar um registro \n-2 Para buscar um registro\n-3 Para sair");
        switch (switcher)
        {
        case 1:
            insert(tree, data, read_record());
            break;
        case 2:
            search(tree, data);
            break;
        }
    }
    close_datafile(data);
    close_tree(tree);
    return 0;
}
void populate(BTREE *tree, FILE *data)
{
    int NUSP;
    char *name, surname, course;
    for (NUSP = 0; NUSP <= N_RECORDS; NUSP++)
    {
        RECORD *record;
        name = randon_string();
        surname = randon_string();
        course = randon_string();
        record = create_record(NUSP, name, surname, course, randon_number());
        insert(tree, data, record);
        free(record);
        free(name);
        free(surname);
        free(course);
    }
}
void insert(BTREE *tree, FILE *data, RECORD *record)
{
    long RNN;
    KEY_VALUE *key;
    if (search_tree(tree, record->NUSP) != -1 //testa se já não existe o registro na arvore
    )
    {
        printf("\nRegistro já existente!\n");
        return;
    }
    RNN = insert_register(record, data);
    key = key_value_creator(record->NUSP, RNN);
    insert_tree(tree, key);
    free(key);
}

void search(BTREE *tree, FILE *data)
{
    int NUSP;
    long RNN;
    RECORD *record;
    printf("\nInsira o número USP a ser buscado: ");
    scanf(" %d", &NUSP);
    RNN = search_tree(tree, NUSP);
    record = search_record(RNN, data);
    print_record(record);
    free(record);
}