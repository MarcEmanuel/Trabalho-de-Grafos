#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Aresta
{
    int origem;
    int destino;
    int peso;
} typedef Aresta;

int encontrarConjunto(int v, int conjunto[])
{
    if (conjunto[v] != v)
    {
        conjunto[v] = encontrarConjunto(conjunto[v], conjunto);
    }
    return conjunto[v];
}

void unirConjuntos(int u, int v, int conjunto[])
{
    int origemU = encontrarConjunto(u, conjunto);
    int origemV = encontrarConjunto(v, conjunto);
    conjunto[origemU] = origemV;
}

int compararArestasOrigem(const void *a, const void *b)
{
    Aresta *a1 = (Aresta *)a;
    Aresta *a2 = (Aresta *)b;
    if (a1->origem == a2->origem)
    {
        return a1->destino - a2->destino;
    }
    return a1->origem - a2->origem;
}

int compararArestas(const void *a, const void *b)
{
    Aresta *a1 = (Aresta *)a;
    Aresta *a2 = (Aresta *)b;
    return a1->peso - a2->peso;
}

void imprimirSaida(Aresta AGM[], int custo, int numVertices, int emOrdemAGM, int imprimirTerminal, FILE *arquivoSaida)
{
    if (emOrdemAGM)
    {
        qsort(AGM, numVertices - 1, sizeof(Aresta), compararArestasOrigem);
    }

    if (imprimirTerminal)
    {
        if (emOrdemAGM)
        {
            for (int i = 0; i < numVertices - 1; i++)
            {
                printf("(%d,%d) ", AGM[i].origem, AGM[i].destino);
            }
            printf("\n");
            return;
        }
        printf("%d\n", custo);
    }
    else
    {
        if (emOrdemAGM)
        {
            for (int i = 0; i < numVertices - 1; i++)
            {
                fprintf(arquivoSaida, "(%d,%d) ", AGM[i].origem, AGM[i].destino);
            }
            fclose(arquivoSaida);
        }
        else
        {
            fprintf(arquivoSaida, "%d\n", custo);
            fclose(arquivoSaida);
        }
    }
}

void Kruskal(Aresta arestas[], Aresta AGM[], int *custo, int numVertices, int numArestas)
{
    int conjunto[numVertices + 1];

    for (int i = 0; i <= numVertices; i++)
    {
        conjunto[i] = i;
    }
    qsort(arestas, numArestas, sizeof(Aresta), compararArestas);

    int numArestasAGM = 0;
    for (int i = 0; i < numArestas; i++)
    {
        int origem = arestas[i].origem;
        int destino = arestas[i].destino;
        int peso = arestas[i].peso;

        if (encontrarConjunto(origem, conjunto) != encontrarConjunto(destino, conjunto))
        {
            AGM[numArestasAGM++] = arestas[i];
            (*custo) += arestas[i].peso;
            unirConjuntos(origem, destino, conjunto);
        }
    }
}

void ajuda()
{
    printf("-o <arquivo> : redireciona a saída para o arquivo\n");
    printf("-f <arquivo> : indica o arquivo que contém o grafo de entrada\n");
    printf("-s : mostra a solução (em ordem crescente)\n");
    printf("-i : vértice inicial\n");
}

int verificarArgumentos(int argc, char *argv[], int *imprimirTerminal, int *emOrdemAGM, FILE **arquivoEntrada, FILE **arquivoSaida)
{
    if (argc < 2)
    {
        printf("Por favor, use por exemplo: %s -f arquivo-entrada.dat\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            ajuda();
            return 1;
        }
        else if (strcmp(argv[i], "-o") == 0)
        {
            *imprimirTerminal = 0;
            if (i < argc - 1)
            {
                char *nomeArquivo = argv[i + 1];
                *arquivoSaida = fopen(nomeArquivo, "w");

                if (*arquivoSaida == NULL)
                {
                    printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
                    return 1;
                }
                i++;
            }
            else
            {
                printf("Erro: Faltando nome do arquivo de saída depois do argumento -o\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            if (i < argc - 1)
            {
                char *nomeArquivo = argv[i + 1];
                *arquivoEntrada = fopen(nomeArquivo, "r");

                if (*arquivoEntrada == NULL)
                {
                    printf("Erro ao abrir o arquivo %s.\n", nomeArquivo);
                    return 1;
                }
                i++;
            }
            else
            {
                printf("Erro: Faltando nome do arquivo de entrada depois do argumento -f\n");
                return 1;
            }
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            *emOrdemAGM = 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int numVertices, numArestas, vertice1, vertice2, pesoAresta, custo = 0;
    int emOrdemAGM = 0,
        imprimirTerminal = 1;
    FILE *arquivoEntrada = NULL, *arquivoSaida = NULL;

    if (verificarArgumentos(argc, argv, &imprimirTerminal, &emOrdemAGM, &arquivoEntrada, &arquivoSaida) != 0)
    {
        return 1;
    }

    fscanf(arquivoEntrada, "%d %d", &numVertices, &numArestas);
    Aresta arestas[numArestas];
    Aresta AGM[numVertices - 1];

    int i = 0;
    while (fscanf(arquivoEntrada, "%d %d %d", &vertice1, &vertice2, &pesoAresta) != EOF)
    {
        arestas[i++] = (Aresta){vertice1, vertice2, pesoAresta};
    }

    Kruskal(arestas, AGM, &custo, numVertices, numArestas);
    imprimirSaida(AGM, custo, numVertices, emOrdemAGM, imprimirTerminal, arquivoSaida);

    fclose(arquivoEntrada);
    return 0;
}
