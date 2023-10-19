#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Estrutura para representar um vértice
struct No {
    int vertice;
    int peso;
    struct No* prox;
} typedef No;

// Estrutura para representar uma aresta
struct ouNO {
    int vim, vou, peso;
} typedef ouNO;

// Estrutura para representar um nó no heap (fila de prioridade)
struct HeapNode {
    int vertice;
    int peso;
};

// Função auxiliar para criar um heap vazio
struct HeapNode* createHeap(int capacity) {
    return (struct HeapNode*)malloc(capacity * sizeof(struct HeapNode));
}

// Função auxiliar para trocar dois elementos em um heap
void swapHeapNodes(struct HeapNode* x, struct HeapNode* y) {
    struct HeapNode temp = *x;
    *x = *y;
    *y = temp;
}

// Função para corrigir o heap após adicionar um novo elemento
void heapifyUp(struct HeapNode* heap, int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (heap[index].peso < heap[parentIndex].peso) {
            swapHeapNodes(&heap[index], &heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

// Função para adicionar um elemento ao heap
void insert(struct HeapNode* heap, int* heapSize, int vertice, int peso) {
    heap[*heapSize].vertice = vertice;
    heap[*heapSize].peso = peso;
    heapifyUp(heap, (*heapSize)++);
}

// Função auxiliar para corrigir o heap após remover um elemento
void heapifyDown(struct HeapNode* heap, int heapSize, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heapSize && heap[left].peso < heap[smallest].peso) {
        smallest = left;
    }

    if (right < heapSize && heap[right].peso < heap[smallest].peso) {
        smallest = right;
    }

    if (smallest != index) {
        swapHeapNodes(&heap[index], &heap[smallest]);
        heapifyDown(heap, heapSize, smallest);
    }
}

// Função para extrair o elemento mínimo (o topo) do heap
struct HeapNode extractMin(struct HeapNode* heap, int* heapSize) {
    struct HeapNode min = heap[0];
    heap[0] = heap[--(*heapSize)];
    heapifyDown(heap, *heapSize, 0);
    return min;
}

// Função para verificar se o heap está vazio
bool isHeapEmpty(int heapSize) {
    return heapSize == 0;
}

void addNo(No* no[], int x1, int x2, int peso) {
    No* new = (No*)malloc(sizeof(No));
    new->vertice = x2;
    new->peso = peso;
    new->prox = NULL;

    if (no[x1] == NULL) {
        no[x1] = new;
        return;
    } else {
        No* Aux = no[x1];
        while (Aux->prox != NULL) {
            Aux = Aux->prox;
        }
        Aux->prox = new;
    }
}

int compararVertices(const void* i, const void* j) {
    struct ouNO* ir = (struct ouNO*)i;
    struct ouNO* volta = (struct ouNO*)j;
    if (ir->vim == volta->vim) {
        return (ir->vou - volta->vou);
    }
    return (ir->vim - volta->vim);
}

void MostrarCaminho(ouNO vertices[], bool ordem, int numVert, int cost, bool mostrarTela, FILE* out) {
    int i;
    if (ordem) {
        qsort(vertices, numVert - 1, sizeof(ouNO), compararVertices);

        if (mostrarTela) {
            for (i = 0; i < numVert - 1; i++) {
                printf("(%d,%d) ", vertices[i].vim, vertices[i].vou);
            }
        } else {
            for (i = 0; i < numVert - 1; i++) {
                fprintf(out, "(%d,%d) ", vertices[i].vim, vertices[i].vou);
            }
            fclose(out);
        }
    }
    if (!ordem && mostrarTela == true) {
        printf("%d\n", cost);
    }
    if (!ordem && !mostrarTela) {
        fprintf(out, "%d\n", cost);
        fclose(out);
    }
}

bool Visitas(int vetor[], int num) {
    int i;
    for (i = 0; i <= num; i++) {
        if (vetor[i] == false)
            return false;
    }
    return true;
}

void Prim(No* no[], ouNO ouno[], int inicioVertice, int tamVertice, int* aux) {
    int count = 0;
    int i, prox, pesin, va, cost = 0;

    int* peso = (int*)malloc(tamVertice * sizeof(int));
    int* visitado = (int*)malloc(tamVertice * sizeof(int));
    int* proximo = (int*)malloc(tamVertice * sizeof(int));

    for (i = 0; i <= tamVertice; i++) {
        peso[i] = 99999;
        visitado[i] = false;
        proximo[i] = -1;
    }

    peso[inicioVertice] = 0;

    // Crie um heap mínimo para selecionar a próxima aresta
    int heapSize = 0;
    struct HeapNode* heap = createHeap(tamVertice);

    // Insira o vértice de início no heap
    insert(heap, &heapSize, inicioVertice, peso[inicioVertice]);

    while (!isHeapEmpty(heapSize)) {
        // Extraia o vértice de menor peso do heap
        struct HeapNode minNode = extractMin(heap, &heapSize);
        va = minNode.vertice;

        if (proximo[va] != -1) {
            cost += peso[va];

            ouno[count].vim = proximo[va];
            ouno[count].vou = va;
            ouno[count].peso = peso[va];
            count++;
        }

        No* aux = no[va];
        while (aux != NULL) {
            prox = aux->vertice;
            pesin = aux->peso;

            if (!visitado[prox] && peso[prox] > pesin) {
                peso[prox] = pesin;
                proximo[prox] = va;

                // Insira o vértice no heap
                insert(heap, &heapSize, prox, pesin);
            }
            aux = aux->prox;
        }
    }

    *aux = cost;
    free(peso);
    free(visitado);
    free(proximo);
    free(heap);
}

void help() {
    printf("-o <arquivo> : redireciona a saida para o arquivo\n");
    printf("-f <arquivo> : indica o arquivo que contem o grafo de entrada\n");
    printf("-s : mostra a solucao (em ordem crescente)\n");
    printf("-i : vertice inicial\n");
}

int Parametro(int argc, char* argv[], bool* mostrarCmd, bool* ordem, int* iniVertice, FILE** input, FILE** output) {
    int i;
    if (argc < 2) {
        printf("Precione -h para saber informações\n");
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
                printf("Falta o arquivo\n");
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
        if (strcmp(argv[i], "-s") == 0) {
            *ordem = true;
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
    bool ordem = false, mostrarCmd = true;
    FILE* inputFile = NULL, *outputFile = NULL;

    if (Parametro(argc, argv, &mostrarCmd, &ordem, &iniVertice, &inputFile, &outputFile) != 1) {
        return 0;
    }

    fscanf(inputFile, "%d %d", &numVertex, &numArestas);
    No* no[numVertex + 1];
    ouNO ouno[numVertex - 1];
    for (int i = 0; i <= numVertex; i++) {
        no[i] = NULL;
    }
    while (fscanf(inputFile, "%d %d %d", &x1, &x2, &peso) != EOF) {
        addNo(no, x1, x2, peso);
        addNo(no, x2, x1, peso);
    }

    Prim(no, ouno, iniVertice, numVertex, &cost);
    MostrarCaminho(ouno, ordem, numVertex, cost, mostrarCmd, outputFile);

    fclose(inputFile);
    return 0;
}
