#ifndef B_TREE_H
#define B_TREE_H

#include <stdlib.h>
#include <stdio.h>

#define ORDER 250

// Estrutura que: guarda a chave (NUSP) e o RNN do registro equivalente no arquivo original
typedef struct key_value{
    long RNN; //8b
    int NUSP; // 4b
} KEY_VALUE; //tamanho 16 devido ao alinhamento de variaveis

// Nós da arvore
typedef struct page {
int is_leaf; //4b, se a pagina for folha = 1, senão = 0
int n_keys; //4b
long RNN;
long children_RNN[ORDER]; // (ordem)*8 ponteiro
KEY_VALUE *keys[ORDER-1]; // (ordem-1)*8 ponteiro
} PAGE; //24b

//Estrutura que guarda a arvore
typedef struct btree{
long count;
long root;
FILE* file;
char dump[99];//variavel para o cabeçalho da arvore ocupar uma página
} BTREE;

//Estrutura retornada pela busca, indica o RNN da página em que o elemento está e sua posição no vetor de chaves
typedef struct search_result{
    long page_RNN;
    int key_position;
}SEARCH_RESULT;



BTREE *start_tree(); //inicia ou carrega a arvore
void close_tree(BTREE *btree); //salva o cabeçalho da arvore e libera a estrutura;
void insert(BTREE *tree, KEY_VALUE *key); //insere um conjunto NUSP/RNN na arvore
long search(BTREE *tree, int NUSP); //Busca por um numero usp
#endif