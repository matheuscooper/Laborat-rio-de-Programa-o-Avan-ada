#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

//Matheus Souza de Oliveira Matrícula: 22153438

//Estrutura de Lista de Vizinhos
typedef struct lista_vizinhos_t {
    int vizinho;
    struct lista_vizinhos_t *proximo;
} lista_vizinhos_t;

typedef struct {
    int id;
    double pos_x;
    double pos_y;
    struct lista_vizinhos_t *vizinhos;
    bool pacote_enviado; 
} no_t;

typedef no_t* grafo_t;


//Estrutura de Eventos
typedef struct event_t {
    double tempo; 
    int alvo;
    int tipo;
}event_t; 

typedef struct lista_eventos_t{
    event_t *evento;
    struct lista_eventos_t *prox; 
}lista_eventos_t;

lista_eventos_t *lista_eventos = NULL;

bool lista_eventos_adicionar_ordenado(event_t *evento, lista_eventos_t **lista){
    lista_eventos_t *item_novo= malloc(sizeof(lista_eventos_t)); 
    if( item_novo == NULL) return false;
        item_novo->evento = evento;
        item_novo->prox = NULL; 

    if (*lista == NULL) {
        *lista = item_novo;
    } else if ((*lista)->evento->tempo > item_novo->evento->tempo) {
        item_novo->prox = *lista;
        *lista = item_novo;
     }
     else{
        lista_eventos_t *item_atual = *lista;
        while((item_atual->prox != NULL) && item_atual->prox->evento->tempo < item_novo->evento->tempo){
            item_atual = item_atual->prox; 
        }
        item_novo->prox = item_atual->prox; 
        item_atual->prox = item_novo; 
     }

}

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

grafo_t grafo_criar(int tam) {
    grafo_t grafo = (grafo_t)malloc(sizeof(no_t) * tam);
    for (int i = 0; i < tam; i++) {
        grafo[i].id = i;
        grafo[i].vizinhos = NULL;
        grafo[i].pacote_enviado = false; 
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

void simulacao_iniciar(lista_eventos_t **lista, grafo_t grafo){
    lista_eventos_t *evento;
    while(*lista!=NULL){
        evento = *lista; 
        printf("[%3.6f] Nó %d recebeu pacote.\n",evento->evento->tempo, evento->evento->alvo);

        no_t *no_atual = &grafo[evento->evento->alvo];
        if(no_atual->pacote_enviado == false){
            no_atual->pacote_enviado = true;
            
            lista_vizinhos_t* vizinhos = no_atual->vizinhos;
            if(vizinhos!= NULL){
                do{ 
                    printf("\t--> Repassando pacote  para o nó %d ...\n", vizinhos->vizinho);
                    event_t * event = malloc(sizeof(event_t));
                    event->alvo = vizinhos->vizinho;
                    event->tipo = 1;
                    event->tempo = evento->evento->tempo + (0.1 + (vizinhos->vizinho * 0.01)); 
                    lista_eventos_adicionar_ordenado(event, lista); 
                    vizinhos = vizinhos->proximo;
                }while(vizinhos!=NULL);
            }

        }
        *lista = (*lista)->prox;
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
        grafo[i].pacote_enviado = false; 
        grafo[i].vizinhos = NULL; 
    }
    fclose(arquivo);
    grafo_atualizar_vizinhos(tam, raio_comunicacao, grafo);

    lista_eventos_t *evento_canonico_lista = NULL; 
    event_t *evento_novo = malloc(sizeof(event_t));
    evento_novo->alvo = 0;
    evento_novo->tempo = 1; 
    evento_novo->tipo = 1; 


    lista_eventos_adicionar_ordenado(evento_novo, &evento_canonico_lista);
    simulacao_iniciar(&evento_canonico_lista, grafo); 
    return 0;
}