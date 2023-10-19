#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INF 99999999
#define NULO -1
#define TRUE 1
#define FALSE 0

typedef struct {
    int v1, v2, w;
} Aresta;

int n, m;
Aresta *arestas;
int *d, *p;

void init(int s) {
    for (int i = 0; i < n; i++) {
        d[i] = INF;
        p[i] = NULO;
    }
    d[s] = 0;
}

void calculo(int u, int v, int w) {
    if (d[v] > (d[u] + w)) {
        d[v] = (d[u] + w);
        p[v] = u;
    }
}

void caminho(int s, int v) {
    if (v == s) {
        printf("  %d  ", s + 1);
    } else if (p[v] == NULO) {
        printf("\nNão existe caminho %d a %d\n", s + 1, v + 1);
    } else {
        caminho(s, p[v]);
        printf("  %d  ", v + 1);
    }
}

int bellmanFord(int s) {
    init(s);
    for (int i = 0; i < (n - 1); i++) {
        for (int j = 0; j < m; j++) {
            int v1 = arestas[j].v1;
            int v2 = arestas[j].v2;
            int w = arestas[j].w;
            calculo(v1, v2, w);
        }
    }

    for (int j = 0; j < m; j++) {
        int v1 = arestas[j].v1;
        int v2 = arestas[j].v2;
        int w = arestas[j].w;
        if (d[v2] > (d[v1] + w)) {
            return FALSE;
        }
    }
    return TRUE;
}

int main(int argc, char *argv[]) {
    int i, s, v;

    if (argc == 1) {
        printf("\nAdicione um texto de entrada! (ex: -f entrada.txt)\n");
        return 1;
    }

    int opcao;
    FILE *file = NULL;

    while ((opcao = getopt(argc, argv, "hf:")) != -1) {
        switch (opcao) {
            case 'h':
                printf("\nUse o parâmetro -f para colocar a entrada. (ex: -f entrada.txt)\n");
                return 0;
            case 'f':
                file = fopen(optarg, "r");
                if (file == NULL) {
                    printf("Arquivo vazio ou inexistente.\n");
                    return 1;
                }
                fscanf(file, "%d %d", &n, &m);

                arestas = (Aresta *)malloc(m * sizeof(Aresta));
                d = (int *)malloc(n * sizeof(int));
                p = (int *)malloc(n * sizeof(int));

                for (i = 0; i < m; i++) {
                    fscanf(file, "%d %d %d", &arestas[i].v1, &arestas[i].v2, &arestas[i].w);
                    arestas[i].v1--;
                    arestas[i].v2--;
                }
                fclose(file);
                break;
        }
    }

    printf("\nDigite o vértice inicial: ");
    scanf("%d", &s);

    printf("\nDigite o vértice final: ");
    scanf("%d", &v);

    if (bellmanFord(s - 1) == TRUE) {
        printf("\nO menor caminho de (%d) para (%d) é:\n\n", s, v);
        caminho(s - 1, v - 1);
        printf("\n\nA distância mínima entre os vértices é %d\n", d[v - 1]);
    } else {
        printf("\nNão existe caminho de (%d) para (%d)\n", s, v);
    }

    free(arestas);
    free(d);
    free(p);
    
    return 0;
}
