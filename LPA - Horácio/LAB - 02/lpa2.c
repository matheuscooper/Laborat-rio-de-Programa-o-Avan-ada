#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
    char nome[51];
    long long int cpf;
    int idade;
} pessoa_t;

typedef struct nodo {
    pessoa_t *pessoa;
    struct nodo *proximo;
} nodo_t;

typedef nodo_t lista_pessoas_t;

typedef lista_pessoas_t** tabela_hash_t;

int tabela_hash_tam;

int tabela_hash_pessoas_funcao(pessoa_t *pessoa) {
    return pessoa->cpf % tabela_hash_tam;
}

bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t **lista) {
    nodo_t *novo_nodo = (nodo_t*)malloc(sizeof(nodo_t));
    if (novo_nodo == NULL) {
        return false;
    }
    novo_nodo->pessoa = pessoa;
    novo_nodo->proximo = *lista;
    *lista = novo_nodo;
    return true;
}

void lista_pessoas_listar(lista_pessoas_t *lista) {
    nodo_t *atual = lista;
    while (atual != NULL) {
        printf("Nome: %s, CPF: %lld, Idade: %d\n", atual->pessoa->nome, atual->pessoa->cpf, atual->pessoa->idade);
        atual = atual->proximo;
    }
}

tabela_hash_t tabela_hash_pessoas_criar() {
    tabela_hash_t tabela = (tabela_hash_t)malloc(sizeof(lista_pessoas_t*) * tabela_hash_tam);
    for (int i = 0; i < tabela_hash_tam; i++) {
        tabela[i] = NULL;
    }
    return tabela;
}

bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash) {
    int indice = tabela_hash_pessoas_funcao(pessoa);
    return lista_pessoas_adicionar(pessoa, &tabela_hash[indice]);
}

void tabela_hash_pessoas_listar(tabela_hash_t tabela_hash) {
    for (int i = 0; i < tabela_hash_tam; i++) {
        printf("Posição %d:\n", i);
        lista_pessoas_listar(tabela_hash[i]);
    }
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Uso: %s <tamanho_tabela> <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    sscanf(argv[1], "%d", &tabela_hash_tam);

    tabela_hash_t tabela_hash = tabela_hash_pessoas_criar();

    FILE *arq_in = fopen(argv[2], "r");
    if (arq_in == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    pessoa_t *pessoa = malloc(sizeof(pessoa_t));
if (pessoa == NULL) {
    printf("Erro ao alocar memória para a pessoa.\n");
    exit(1);
}

while (fscanf(arq_in, "%s %lld %d", pessoa->nome, &pessoa->cpf, &pessoa->idade) == 3) {
    tabela_hash_pessoas_adicionar(pessoa, tabela_hash);

    pessoa = malloc(sizeof(pessoa_t));  // Aloca uma nova pessoa para a próxima iteração
    if (pessoa == NULL) {
        printf("Erro ao alocar memória para a pessoa.\n");
        exit(1);
    }
}


    fclose(arq_in);

    tabela_hash_pessoas_listar(tabela_hash);

    for (int i = 0; i < tabela_hash_tam; i++) {
        while (tabela_hash[i] != NULL) {
            nodo_t *temp = tabela_hash[i];
            tabela_hash[i] = tabela_hash[i]->proximo;
            free(temp);
        }
    }
    free(tabela_hash);

    return 0;
}
