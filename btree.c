#include "btree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*INSERT
PAGE* load_page(int RNN);
void save_page(PAGE* page);
int search_child(PAGE* page,int NUSP);
insert(value_key,*PAGE);
    split()*/

// funçoes de conversão
long offset_to_RNN(long offset)
{
    return (offset - sizeof(BTREE)) / sizeof(PAGE);
}
long RNN_to_offset(long RNN)
{
    return (RNN * sizeof(PAGE)) + sizeof(BTREE);
}
//funçoes de manipulacao de paginas
PAGE *load_page(long RNN, BTREE *tree)
{
    int offset = RNN_to_offset(RNN);
    PAGE *page = malloc(sizeof(PAGE));
    fseek(tree->file, offset, SEEK_SET);
    fread(page, sizeof(PAGE), 1, tree->file);
    return page;
}
void save_page(PAGE *page, BTREE *tree)
{
    int offset = RNN_to_offset(page->RNN);
    fwrite(page, sizeof(PAGE), 1, tree->file);
    //free(page);
}
//inicia uma arvore
KEY_VALUE* key_value_creator(int NUSP,long RNN){
    KEY_VALUE* key = malloc(sizeof(KEY_VALUE));
    key->NUSP = NUSP;
    key->RNN = RNN;
    return key;

}
BTREE *start_tree()
{ //Posivel bug, ao ler a informação do arquivo o ponteiro file precisar ser refeito
    BTREE *btree = malloc(sizeof(BTREE));
    btree->count = 0;
    btree->root = -1;
    do
    {
        btree->file = fopen("index.dat", "r+");
        if (btree->file == NULL)
        {
            btree->file = fopen("index.dat  ", "w");      /* Se nao existir, cria o arquivo de dados */
            fwrite(btree, sizeof(BTREE), 1, btree->file); //salva a estrutura btree no inicio do arquivo

            PAGE *root = new_page(btree); //aloca a primeira pagina "root"
            btree->root = root->RNN;
            free(root);

            fclose(btree->file);
            btree->file = NULL;
        }
    } while (btree->file == NULL);
    fread(btree, sizeof(BTREE), 1, btree->file);

    return btree;
}
void close_tree(BTREE *btree)
{
    fseek(btree->file, 0, SEEK_SET);
    fwrite(btree, sizeof(BTREE), 1, btree->file);
    free(btree);
}

//inicia uma nova pagina, salva a pagina no arquivo de dados e returna um ponteiro para o struct
PAGE *new_page(BTREE *tree)
{
    int i = 0;
    PAGE *page = malloc(sizeof(PAGE));
    page->n_keys = 0;
    page->is_leaf = 1;
    //indicando que todos os ponteiros são nulos
    while (i < ORDER)
    {
        page->children_RNN[i] = -1;
    }
    fseek(tree->file, 0, SEEK_END);
    page->RNN = offset_to_RNN(ftell(tree->file));
    fwrite(page, sizeof(PAGE), 1, tree->file);
    return page;
}
void insert_key(PAGE *page, KEY_VALUE *key, long RNN_child) //adiciona a chave e o ponteiro para os maiores que la
{
    int i = 0;
    while (page->keys[i]->NUSP < key->NUSP && i < ORDER - 2 && page->keys[i]->NUSP) //percorre o vetor de chavere salva a posição anterio a que a chave deve ser adicionada;
    {
        i++;
    }
    //faz o shift e adiciona a chave->
    //o algoritmo garante que eu vou ter pelo menos um espaço livre no vetor de chaves e de filhos
    if (i != ORDER - 3)
        memcpy(page->keys[i + 1], page->keys[i + 2], (ORDER - 3 - i) * sizeof(KEY_VALUE *));
    page->keys[i + 1] = key;
    if (RNN_child != -1)
    {
        memcpy(page->children_RNN[i + 2], page->children_RNN[i + 3], (ORDER - 4 - i) * sizeof(long));
        page->children_RNN[i + 2] = RNN_child;
    }
    page->n_keys++;
}
long find_child(PAGE *page, int key)
{
    int i = 0;
    while (page->keys[i]->NUSP > key && i < ORDER - 2)
    {
        i++;
    }
    return page->children_RNN[i + 1];
}
r_insert(BTREE *tree, KEY_VALUE *key, PAGE *root, PAGE *child)
{
    if (child->is_leaf)
    {
        insert_key(child, key, -1);
    }
    else
    {
        PAGE *new_child = load_page(find_child(child, key->NUSP), tree);
        r_insert(tree, key, child, new_child);
        save_page(new_child, tree);
        free(new_child);
    }
    if (child->n_keys == ORDER - 1)
    {
        split(tree, root, child);
    }
    //verificar se child ta cheio, se estiver split e depois fechar os ponteiros
}
void split_root(BTREE *tree, PAGE *root) //faz o split quando a page é a root da arvore,deixando a ultima chave da primeira metade na root
{
    //criando paginas filhas
    PAGE *smaller = new_page(tree);
    PAGE *bigger = new_page(tree);
    //copiando as chaves para as paginas filhas
    memcpy(smaller->keys[0], root->keys[0], ((ORDER / 2) - 1) * sizeof(KEY_VALUE *));
    memcpy(smaller->children_RNN[0], root->children_RNN[0], (ORDER / 2) * sizeof(long));

    memcpy(bigger->keys[0], root->keys[(ORDER / 2) - 1], ((ORDER - 1) / 2) * sizeof(KEY_VALUE *));
    memcpy(bigger->children_RNN[0], root->keys[(ORDER / 2)], ((ORDER + 1) / 2) * sizeof(long));

    //movendo a chave "promovida " para a posição certa e adicionando o RNN das novas filhas
    root->keys[0] = root->keys[(ORDER / 2) - 1];
    root->children_RNN[0] = smaller->RNN;
    root->children_RNN[1] = bigger->RNN;

    for (int i = 1; i < ORDER - 2; i++)
    { //atribuindo null as chaves que foram movidas
        root->keys[i] = NULL;
    }
    for (int i = 2; i < ORDER - 1; i++)
    { //zerando os RNN dos filhos
        root->children_RNN[i] = -1;
    }
    //salvando alteraçoes no arquivo
    save_page(root, tree);
    save_page(smaller, tree);
    save_page(bigger, tree);
    //liberando paginas
    free(root);
    free(smaller);
    free(bigger);
}
void split(BTREE *tree, PAGE *father, PAGE *page) //faz o split da page,promovendo a ultima chave da primeira metade
{
    PAGE *bigger = new_page(tree);

    memcpy(bigger->keys[0], page->keys[ORDER / 2], ((ORDER - 1) / 2) * sizeof(PAGE *));
    insert_key(father, page->keys[(ORDER / 2) - 1], bigger->RNN);
    for (int i = (ORDER / 2) - 1; i < ORDER - 2; i++)
    {
        page->keys[i] = NULL;
    }
    memcpy(bigger->children_RNN[0], page->children_RNN[ORDER / 2], ((ORDER + 1) / 2) * sizeof(long));
    for (int i = ORDER / 2; i < ORDER - 1; i++)
    {
        page->children_RNN[i] = -1;
    }
    save_page(father, tree);
    save_page(page, tree);
    free(father);
    free(page);
}
void insert_tree(BTREE *tree, KEY_VALUE *key)
{
    PAGE *root = load_page(tree->root, tree);
    if (root->is_leaf)
    {
        insert_key(root, key, -1);
    }
    else
    {
        PAGE *child = load_page(find_child(root, key->NUSP), tree);
        r_insert(tree, key, root, child);
        free(child);
    }
    if (root->n_keys == ORDER - 1)
    {
        split_root(tree, root);
    }
    save_page(root, tree);
    free(root);
}
long binary_serch(KEY_VALUE arr[], int l, int r, int x) // inspirado em https://www.geeksforgeeks.org/binary-search/
{
    if (r >= l)
    {
        int mid = l + (r - l) / 2;

        if (arr[mid].NUSP == x)
            return arr[mid].RNN;

        if (arr[mid].NUSP > x)
            return binary_search(arr, l, mid - 1, x);

        return binary_search(arr, mid + 1, r, x);
    }

    return -1;
}
long search_tree(BTREE *tree, int NUSP)
{
    long child = tree->root;
    PAGE *page;
    long RNN;
    while (child != -1)
    {
        page = load_page(child, tree);
        RNN = binary_search(page, NUSP);
        if (RNN != -1)
        {
            free(page);
            return RNN;
        }
        else
        {
            child = find_child(page, NUSP);
            free(page);
        }
    }
    return -1;
}