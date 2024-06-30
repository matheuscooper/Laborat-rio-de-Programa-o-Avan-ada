#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
// Estrutura para representar uma pessoa
typedef struct {
    char nome[51];
    long long int cpf;
    int idade;
} pessoa_t;

// Estrutura para representar um nó da lista encadeada
typedef struct nodo {
    pessoa_t pessoa;
    struct nodo *proximo;
} nodo_t;

// Tipo para representar uma lista encadeada
typedef nodo_t* lista_pessoas_t;

// Tipo para representar a tabela hash (vetor de listas)
typedef lista_pessoas_t* tabela_hash_t;

int tabela_hash_tam; // Tamanho da tabela hash

// Função para calcular a função hash
int tabela_hash_pessoas_funcao(pessoa_t *pessoa) {
    return pessoa->cpf % tabela_hash_tam;
}

// Função para adicionar uma pessoa a uma lista encadeada
bool lista_pessoas_adicionar(pessoa_t *pessoa, lista_pessoas_t *lista) {
    nodo_t *novo_nodo = (nodo_t*)malloc(sizeof(nodo_t));
    if (novo_nodo == NULL) {
        return false; // Falha na alocação de memória
    }
    novo_nodo->pessoa = *pessoa;
    novo_nodo->proximo = *lista;
    *lista = novo_nodo;
    return true;
}

// Função para listar pessoas em uma lista encadeada
void lista_pessoas_listar(lista_pessoas_t lista) {
    nodo_t *atual = lista;
    while (atual != NULL) {
        printf("Nome: %s, CPF: %lld, Idade: %d\n", atual->pessoa.nome, atual->pessoa.cpf, atual->pessoa.idade);
        atual = atual->proximo;
    }
}

// Função para criar a tabela hash
tabela_hash_t tabela_hash_pessoas_criar() {
    tabela_hash_t tabela = (lista_pessoas_t*)malloc(sizeof(lista_pessoas_t) * tabela_hash_tam);
    for (int i = 0; i < tabela_hash_tam; i++) {
        tabela[i] = NULL; // Inicializa todas as posições com NULL (sem colisões)
    }
    return tabela;
}

// Função para adicionar pessoa à tabela hash
bool tabela_hash_pessoas_adicionar(pessoa_t *pessoa, tabela_hash_t tabela_hash) {
    int indice = tabela_hash_pessoas_funcao(pessoa);
    return lista_pessoas_adicionar(pessoa, &tabela_hash[indice]);
}

// Função para listar todas as pessoas na tabela hash
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

    // Lê o tamanho da tabela
    sscanf(argv[1], "%d", &tabela_hash_tam);

    // Cria a tabela hash
    tabela_hash_t tabela_hash = tabela_hash_pessoas_criar();

    // Lê o arquivo de entrada
    FILE *arq_in = fopen(argv[2], "r");
    if (arq_in == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    pessoa_t pessoa;
    while (fscanf(arq_in, "%50[^\t]\t%lld\t%d\n", pessoa.nome, &pessoa.cpf, &pessoa.idade) == 3) {
        tabela_hash_pessoas_adicionar(&pessoa, tabela_hash);
    }

    fclose(arq_in);

    // Lista todas as pessoas na tabela hash
    tabela_hash_pessoas_listar(tabela_hash);

    // Libera a memória alocada
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
