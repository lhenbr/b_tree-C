#ifndef B_TREE_H
#define B_TREE_H

// Estrutura que guarda a chave (NUSP) e o RNN do registro equivalente no arquivo original
typedef struct key_value{
    int NUSP;
    long RNN;
} KEY_VALUE;
// Nós da arvore
typedef struct page {
bool is_leaf;
int n_keys;
long *children_RNN;
KEY_VALUE *keys;

} PAGE;
//Estrutura que guarda a arvore
typedef struct btree{
long root;

} BTREE;

//Estrutura retornada pela busca, indica o RNN da página em que o elemento está e sua posição no vetor de chaves
typedef struct search_result{
    long page_RNN;
    int key_position;
}SEARCH_RESULT;





#endif