#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Incluído para usar tolower()

#define MAX_STR 50

typedef struct Sala {
    char nome[MAX_STR];
    char pista[MAX_STR];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

typedef struct PistaNode {
    char pista[MAX_STR];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

Sala* criarSala(const char *nome, const char *pista) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro de alocacao de memoria para Sala");
        exit(EXIT_FAILURE);
    }
    strncpy(novaSala->nome, nome, MAX_STR - 1);
    novaSala->nome[MAX_STR - 1] = '\0';
    strncpy(novaSala->pista, pista, MAX_STR - 1);
    novaSala->pista[MAX_STR - 1] = '\0';

    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    return novaSala;
}

PistaNode* criarPistaNode(const char *pista) {
    PistaNode *novoNode = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNode == NULL) {
        perror("Erro de alocacao de memoria para PistaNode");
        exit(EXIT_FAILURE);
    }
    strncpy(novoNode->pista, pista, MAX_STR - 1);
    novoNode->pista[MAX_STR - 1] = '\0';
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    return novoNode;
}

PistaNode* inserirPista(PistaNode* raiz, const char *pista) {
    if (raiz == NULL) {
        printf(" -> Pista coletada: \"%s\"\n", pista);
        return criarPistaNode(pista);
    }

    int comparacao = strcmp(pista, raiz->pista);

    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

void explorarSalasComPistas(Sala* mapaRaiz, PistaNode** pistasRaiz) {
    Sala* atual = mapaRaiz;
    char escolha;

    if (atual == NULL) {
        printf("Mapa da mansao vazio.\n");
        return;
    }

    while (atual != NULL) {
        printf("\n======================================================\n");
        printf("Voce esta em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("[PISTA ENCONTRADA!]\n");
            *pistasRaiz = inserirPista(*pistasRaiz, atual->pista);
            atual->pista[0] = '\0'; 
        } else {
             printf("[Sem pistas neste comodo ou ja coletada.]\n");
        }
        printf("------------------------------------------------------\n");

        printf("Escolha o proximo caminho: ");
        if (atual->esquerda) {
            printf("(E)squerda para %s, ", atual->esquerda->nome);
        }
        if (atual->direita) {
            printf("(D)ireita para %s, ", atual->direita->nome);
        }
        printf("(S)air da mansao: ");
        
        scanf(" %c", &escolha);
        escolha = tolower(escolha);

        if (escolha == 's') {
            printf("\nSaindo da mansao...\n");
            break;
        } 
        
        if (escolha == 'e') {
            if (atual->esquerda) {
                atual = atual->esquerda;
            } else {
                printf("Caminho Esgotado! Nao ha comodo a esquerda.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita) {
                atual = atual->direita;
            } else {
                printf("Caminho Esgotado! Nao ha comodo a direita.\n");
            }
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

void liberarPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

void liberarMapa(Sala* raiz) {
    if (raiz != NULL) {
        liberarMapa(raiz->esquerda);
        liberarMapa(raiz->direita);
        free(raiz);
    }
}

int main() {
    Sala *hallEntrada = criarSala("Hall de Entrada", "A chave do escritorio esta com a cozinheira");
    
    Sala *salaEstar = criarSala("Sala de Estar", "O objeto de metal esta na biblioteca");
    Sala *cozinha = criarSala("Cozinha", "O culpado tem um relogio de ouro");

    Sala *biblioteca = criarSala("Biblioteca", "O crime ocorreu apos a meia-noite");
    Sala *escritorio = criarSala("Escritorio", "");
    Sala *dispensa = criarSala("Dispensa", "O lenco de seda foi queimado");
    Sala *quartoMestre = criarSala("Quarto Mestre", "A carta de ameaca foi rasgada em 4");

    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;

    salaEstar->esquerda = biblioteca;
    salaEstar->direita = escritorio;

    cozinha->esquerda = dispensa;
    cozinha->direita = quartoMestre;

    PistaNode *pistasColetadas = NULL;

    printf("--- BEM-VINDO AO DETECTIVE QUEST ---\n");
    printf("Explore a mansao para coletar pistas.\n");

    explorarSalasComPistas(hallEntrada, &pistasColetadas);

    printf("\n======================================================\n");
    printf("--- FIM DA EXPLORACAO ---\n");
    printf("Pistas Coletadas (Ordem Alfabetica):\n");
    if (pistasColetadas == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasColetadas);
    }
    printf("======================================================\n");

    liberarMapa(hallEntrada);
    liberarPistas(pistasColetadas);

    return 0;
}