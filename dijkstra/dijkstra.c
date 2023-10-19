#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct No {
    int vertice;
    int distancia;
    struct No* prox;
} typedef No;

struct HeapNode {
    int vertice;
    int distancia;
};

No* searchMinDistance(No* distancias[], bool visitados[], int numVertices) {
    int i;
    No* agora = NULL;

    for (i = 0; i < numVertices; i++) {
        if (distancias[i] != NULL && distancias[i]->distancia < agora->distancia && visitados[i] == false) {
            agora = distancias[i];
        }
    }
    return agora;
}

// Estrutura para representar um heap mínimo (min-heap)
struct MinHeap {
    struct HeapNode* array;
    int tamanho;
    int capacidade;
};

struct MinHeap* createMinHeap(int capacidade) {
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->tamanho = 0;
    minHeap->capacidade = capacidade;
    minHeap->array = (struct HeapNode*)malloc(capacidade * sizeof(struct HeapNode));
    return minHeap;
}

void swap(struct HeapNode* a, struct HeapNode* b) {
    struct HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct MinHeap* minHeap, int indice) {
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < minHeap->tamanho && minHeap->array[esquerda].distancia < minHeap->array[menor].distancia) {
        menor = esquerda;
    }

    if (direita < minHeap->tamanho && minHeap->array[direita].distancia < minHeap->array[menor].distancia) {
        menor = direita;
    }

    if (menor != indice) {
        swap(&minHeap->array[indice], &minHeap->array[menor]);
        minHeapify(minHeap, menor);
    }
}

struct HeapNode extractMin(struct MinHeap* minHeap) {
    struct HeapNode min = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->tamanho - 1];
    minHeap->tamanho--;
    minHeapify(minHeap, 0);
    return min;
}

void decreaseKey(struct MinHeap* minHeap, int vertice, int novaDistancia) {
    int i;
    for (i = 0; i < minHeap->tamanho; i++) {
        if (minHeap->array[i].vertice == vertice) {
            minHeap->array[i].distancia = novaDistancia;
            break;
        }
    }

    i = (i - 1) / 2;
    while (i >= 0 && minHeap->array[i].distancia > minHeap->array[(i - 1) / 2].distancia) {
        swap(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct MinHeap* minHeap, int vertice) {
    int i;
    for (i = 0; i < minHeap->tamanho; i++) {
        if (minHeap->array[i].vertice == vertice) {
            return true;
        }
    }
    return false;
}

void MostrarCaminho(No* distancia[], int numVertice, bool mostrarCmd, FILE* out) {
    int i;
    if (mostrarCmd == true) {
        for (i = 1; i < numVertice; i++) {
            if (distancia[i] == NULL || distancia[i]->distancia == 99999) {
                printf("%d:-1 ", i);
            } else {
                printf("%d:%d ", i, distancia[i]->distancia);
            }
        }
        printf("\n");
    } else {
        for (i = 1; i < numVertice; i++) {
            if (distancia[i] == NULL) {
                fprintf(out, "(%d:-1) ", i);
            } else {
                fprintf(out, "(%d:%d) ", i, distancia[i]->distancia);
            }
        }
        fprintf(out, "\n");
        fclose(out);
    }
}

void Dijkstra(int numVertex, int grafo[][numVertex], int inicio, bool mostrarCmd, FILE* out) {
    int i, j;
    No* distancia[numVertex];
    bool visited[numVertex];
    struct MinHeap* minHeap = createMinHeap(numVertex);

    for (i = 0; i < numVertex; i++) {
        distancia[i] = NULL;
        visited[i] = false;
    }

    distancia[inicio] = (No*)malloc(sizeof(No));
    distancia[inicio]->vertice = inicio;
    distancia[inicio]->distancia = 0;
    distancia[inicio]->prox = NULL;
    minHeap->array[0].vertice = inicio;
    minHeap->array[0].distancia = 0;
    minHeap->tamanho = 1;

    for (i = 0; i < numVertex - 1; i++) {
        struct HeapNode minNode = extractMin(minHeap);
        int u = minNode.vertice;
        visited[u] = true;

        for (j = 0; j < numVertex; j++) {
            if (!visited[j] && grafo[u][j] && distancia[u]->distancia != 99999 && grafo[u][j] + distancia[u]->distancia < distancia[j]->distancia) {
                distancia[j]->distancia = grafo[u][j] + distancia[u]->distancia;
                decreaseKey(minHeap, j, distancia[j]->distancia);
            }
        }
    }

    MostrarCaminho(distancia, numVertex, mostrarCmd, out);
}

void help() {
    printf("-o <arquivo> : redireciona a saida para o arquivo\n");
    printf("-f <arquivo> : indica o arquivo que contem o grafo de entrada\n");
    printf("-s : mostra a solucao (em ordem crescente)\n");
    printf("-i : vertice inicial\n");
}

int Parametro(int argc, char* argv[], bool* mostrarCmd, int* iniVertice, FILE** input, FILE** output) {
    int i;
    if (argc < 2) {
        printf("precione -h para saber infromacoes\n");
        return 0;
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            help();
            return 0;
        }
        if (strcmp(argv[i], "-o") == 0) {
            *mostrarCmd = false;
            if (i < argc - 1) {
                char* nome = argv[i + 1];
                *output = fopen(nome, "w");
                if (*output == NULL) {
                    printf("Erro ao abrir o arquivo.\n");
                    return 0;
                }
                i++;
            } else {
                printf("falta o arquivo\n");
                return 0;
            }
        }
        if (strcmp(argv[i], "-f") == 0) {
            if (i < argc - 1) {
                char* nome = argv[i + 1];
                *input = fopen(nome, "r");
                if (*input == NULL) {
                    printf("Erro ao abrir o arquivo\n");
                    return 0;
                }
                i++;
            } else {
                printf("Falta o arquivo de entrada\n");
                return 0;
            }
        }
        if (strcmp(argv[i], "-i") == 0) {
            if (i < argc - 1) {
                *iniVertice = atoi(argv[i + 1]);
                i++;
            } else {
                printf("Falta valor inteiro\n");
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int iniVertice = 1, numVertex, numArestas, x1, x2, peso, cost = 0;
    bool mostrarCmd = true;
    FILE* inputFile = NULL, *outputFile = NULL;

    if (Parametro(argc, argv, &mostrarCmd, &iniVertice, &inputFile, &outputFile) != 1) {
        return 0;
    }

    fscanf(inputFile, "%d %d", &numVertex, &numArestas);
    int grafo[numVertex + 1][numVertex + 1];
    memset(grafo, 0, sizeof(grafo));

    while (fscanf(inputFile, "%d %d %d", &x1, &x2, &peso) != EOF) {
        grafo[x1][x2] = peso;
        grafo[x2][x1] = peso;
    }

    Dijkstra(numVertex + 1, grafo, iniVertice, mostrarCmd, outputFile);

    fclose(inputFile);
    return 0;
}
