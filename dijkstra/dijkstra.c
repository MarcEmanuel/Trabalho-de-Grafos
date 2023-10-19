#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode* head;
};

struct Graph {
    int V;
    struct AdjList* array;
};

struct HeapNode {
    int vertice;
    int distancia;
};

typedef struct Graph Graph;
typedef struct HeapNode HeapNode;
typedef struct AdjListNode AdjListNode;
typedef struct AdjList AdjList;

Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));

    for (int i = 0; i < V; i++) {
        graph->array[i].head = NULL;
    }

    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
}

HeapNode* searchMinDistance(HeapNode* distancias, bool visitados[], int numVertices) {
    int minDist = -1;
    HeapNode* minNode = NULL;

    for (int v = 0; v < numVertices; v++) {
        if (!visitados[v] && distancias[v].distancia != -1) {
            if (minDist == -1 || distancias[v].distancia < minDist) {
                minDist = distancias[v].distancia;
                minNode = &distancias[v];
            }
        }
    }
    return minNode;
}

HeapNode* dijkstra(Graph* graph, int inicio) {
    int numVertices = graph->V;
    HeapNode* distancias = (HeapNode*)malloc(numVertices * sizeof(HeapNode));
    bool visitados[numVertices];

    for (int i = 0; i < numVertices; i++) {
        distancias[i].vertice = i;
        distancias[i].distancia = -1;
        visitados[i] = false;
    }

    distancias[inicio].distancia = 0;

    for (int count = 0; count < numVertices - 1; count++) {
        HeapNode* minNode = searchMinDistance(distancias, visitados, numVertices);
        if (minNode == NULL) {
            break;
        }
        int u = minNode->vertice;
        visitados[u] = true;

        AdjListNode* node = graph->array[u].head;
        while (node != NULL) {
            int v = node->dest;
            if (!visitados[v] && (distancias[u].distancia != -1) && (distancias[v].distancia == -1 || distancias[u].distancia + node->weight < distancias[v].distancia)) {
                distancias[v].distancia = distancias[u].distancia + node->weight;
            }
            node = node->next;
        }
    }

    return distancias;
}

void MostrarCaminho(HeapNode* distancias, int numVertice, bool mostrarCmd, FILE* out) {
    if (mostrarCmd) {
        for (int i = 0; i < numVertice; i++) {
            if (i != 0) {
                printf(" ");
            }
            printf("%d:%d", i, distancias[i].distancia);
        }
        printf("\n");
    } else {
        for (int i = 0; i < numVertice; i++) {
            fprintf(out, "(%d:%d) ", i, distancias[i].distancia);
        }
        fprintf(out, "\n");
        fclose(out);
    }
}

void help() {
    printf("-o <arquivo> : redireciona a saída para o arquivo\n");
    printf("-f <arquivo> : indica o arquivo que contém o grafo de entrada\n");
    printf("-s : mostra a solução (em ordem crescente)\n");
    printf("-i : vértice inicial\n");
}

int Parametro(int argc, char* argv[], bool* mostrarCmd, int* iniVertice, FILE** input, FILE** output) {
    int i;
    if (argc < 2) {
        printf("pressione -h para saber informações\n");
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
    Graph* grafo = createGraph(numVertex);

    while (fscanf(inputFile, "%d %d %d", &x1, &x2, &peso) != EOF) {
        addEdge(grafo, x1, x2, peso);
        addEdge(grafo, x2, x1, peso);  // Grafo não direcionado, adicionamos ambas as direções
    }

    HeapNode* distancias = dijkstra(grafo, iniVertice);

    MostrarCaminho(distancias, numVertex, mostrarCmd, outputFile);

    fclose(inputFile);
    return 0;
}
