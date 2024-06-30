#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//Matheus Souza de Oliveira Matrícula: 22153438


typedef struct lista_vizinhos_t {
    int vizinho;
    struct lista_vizinhos_t *proximo;
} lista_vizinhos_t;

typedef struct {
    int id;
    double pos_x;
    double pos_y;
    struct lista_vizinhos_t *vizinhos;
} no_t;

typedef no_t* grafo_t;

bool lista_vizinhos_adicionar(int vizinho, lista_vizinhos_t **lista) {
    lista_vizinhos_t *novo_vizinho = (lista_vizinhos_t*)malloc(sizeof(lista_vizinhos_t));
    if (novo_vizinho == NULL) {
        return false;
    }
    novo_vizinho->vizinho = vizinho;
    novo_vizinho->proximo = *lista;
    *lista = novo_vizinho;
    return true;
}

void lista_vizinhos_imprimir(lista_vizinhos_t *lista) {
    lista_vizinhos_t *atual = lista;
    while (atual != NULL) {
        printf("%d ", atual->vizinho);
        atual = atual->proximo;
    }
    printf("\n");
}

grafo_t grafo_criar(int tam) {
    grafo_t grafo = (grafo_t)malloc(sizeof(no_t) * tam);
    for (int i = 0; i < tam; i++) {
        grafo[i].id = i;
        grafo[i].vizinhos = NULL;
    }
    return grafo;
}

void grafo_atualizar_vizinhos(int tam, double raio_comunicacao, grafo_t grafo) {
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (i != j) {
                double distancia = sqrt(pow(grafo[i].pos_x - grafo[j].pos_x, 2) + pow(grafo[i].pos_y - grafo[j].pos_y, 2));
                if (distancia <= raio_comunicacao) {
                    lista_vizinhos_adicionar(i, &grafo[j].vizinhos);
                }
            }
        }
    }
}

void grafo_imprimir(int tam, grafo_t grafo) {
    for (int i = 0; i < tam; i++) {
        printf("Nó %d:  ", grafo[i].id, grafo[i].pos_x, grafo[i].pos_y);
        lista_vizinhos_imprimir(grafo[i].vizinhos);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE *arquivo = fopen(argv[1], "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    int tam;
    double raio_comunicacao;
    fscanf(arquivo, "%d %lf", &tam, &raio_comunicacao);

    grafo_t grafo = grafo_criar(tam);

    for (int i = 0; i < tam; i++) {
        fscanf(arquivo, "%d %lf %lf", &grafo[i].id, &grafo[i].pos_x, &grafo[i].pos_y);
    }

    grafo_atualizar_vizinhos(tam, raio_comunicacao, grafo);

    grafo_imprimir(tam, grafo);

    fclose(arquivo);
    free(grafo);

    return 0;
}
