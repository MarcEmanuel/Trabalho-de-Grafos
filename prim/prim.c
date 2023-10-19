#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

struct No {
    int vertice;
    int peso;
    struct No* prox;
};

struct ouNO {
    int vim, vou, peso;
};

// Função auxiliar para adicionar uma aresta a uma lista de adjacência
void addNo(struct No** lista, int vertice, int peso) {
    struct No* novo = (struct No*)malloc(sizeof(struct No));
    novo->vertice = vertice;
    novo->peso = peso;
    novo->prox = *lista;
    *lista = novo;
}

// Função auxiliar para liberar a memória de uma lista de adjacência
void liberarLista(struct No* lista) {
    while (lista != NULL) {
        struct No* temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

// Função auxiliar para mostrar o caminho da árvore geradora mínima
void mostrarCaminho(struct ouNO* arestas, int numArestas) {
    printf("Caminho da árvore geradora mínima:\n");
    for (int i = 0; i < numArestas; i++) {
        printf("Aresta (%d,%d) com peso %d\n", arestas[i].vim, arestas[i].vou, arestas[i].peso);
    }
}

// Função para implementar o algoritmo de Prim
void prim(struct No** lista, int numVertices, struct ouNO* arestas) {
    int* pai = (int*)malloc(numVertices * sizeof(int));
    int* pesoMinimo = (int*)malloc(numVertices * sizeof(int));
    bool* naArvore = (bool*)malloc(numVertices * sizeof(bool));

    for (int i = 0; i < numVertices; i++) {
        pai[i] = -1;
        pesoMinimo[i] = INT_MAX;
        naArvore[i] = false;
    }

    int inicioVertice = 0;  // Começando a partir do vértice 0 (você pode escolher o vértice inicial desejado)
    pesoMinimo[inicioVertice] = 0;

    for (int count = 0; count < numVertices - 1; count++) {
        int verticeAtual = -1;
        for (int v = 0; v < numVertices; v++) {
            if (!naArvore[v] && (verticeAtual == -1 || pesoMinimo[v] < pesoMinimo[verticeAtual])) {
                verticeAtual = v;
            }
        }

        if (verticeAtual == -1) {
            // Não foi possível encontrar um vértice, algo deu errado
            break;
        }

        naArvore[verticeAtual] = true;

        struct No* aux = lista[verticeAtual];
        while (aux != NULL) {
            int v = aux->vertice;
            int peso = aux->peso;

            if (!naArvore[v] && peso < pesoMinimo[v]) {
                pai[v] = verticeAtual;
                pesoMinimo[v] = peso;
            }
            aux = aux->prox;
        }
    }

    int numArestas = 0;
    for (int i = 0; i < numVertices; i++) {
        if (pai[i] != -1) {
            arestas[numArestas].vim = pai[i];
            arestas[numArestas].vou = i;
            arestas[numArestas].peso = pesoMinimo[i];
            numArestas++;
        }
    }

    mostrarCaminho(arestas, numArestas);

    free(pai);
    free(pesoMinimo);
    free(naArvore);
}

void help() {
    printf("-f <arquivo> : indica o arquivo que contém o grafo de entrada\n");
}

int main(int argc, char* argv[]) {
    int numVertices, numArestas, x1, x2, peso;
    FILE* inputFile = NULL;

    if (argc < 3) {
        help();
        return 1;
    }

    if (strcmp(argv[1], "-f") != 0) {
        help();
        return 1;
    }

    char* nomeArquivo = argv[2];
    inputFile = fopen(nomeArquivo, "r");
    if (inputFile == NULL) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    fscanf(inputFile, "%d %d", &numVertices, &numArestas);
    struct No** listaAdjacencia = (struct No**)malloc(numVertices * sizeof(struct No*));
    for (int i = 0; i < numVertices; i++) {
        listaAdjacencia[i] = NULL;
    }

    while (fscanf(inputFile, "%d %d %d", &x1, &x2, &peso) != EOF) {
        addNo(&listaAdjacencia[x1], x2, peso);
        addNo(&listaAdjacencia[x2], x1, peso); // Considere arestas bidirecionais
    }

    struct ouNO* arestas = (struct ouNO*)malloc((numVertices - 1) * sizeof(struct ouNO));
    prim(listaAdjacencia, numVertices, arestas);

    for (int i = 0; i < numVertices; i++) {
        liberarLista(listaAdjacencia[i]);
    }
    free(listaAdjacencia);
    free(arestas);
    fclose(inputFile);

    return 0;
}
