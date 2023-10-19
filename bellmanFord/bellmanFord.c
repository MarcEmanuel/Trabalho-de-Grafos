#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INF 9999
#define NULO -1
#define TRUE 1
#define FALSE 0
#define MAX 100

FILE *file;

int grafo[MAX][MAX][2],d[MAX],p[MAX];
int n,m;

void init(int s)
{
    int i;
    for(i=0;i<n;i++)
    {
        d[i]=INF;
        p[i]=NULO;
    }
    d[s]=0;
}

void calculo(int u,int v)
{
    if(d[v]>(d[u]+grafo[u][v][1]))
    {
        d[v]=(d[u]+grafo[u][v][1]);
        p[v]=u;
    }
}

void caminho(int s, int v)
{
    if(v==s){
        printf("  %d  ",s+1);
    }
    
    else if(p[v]==NULO){
        printf("\nNão existe caminho %d a %d \n",s+1,v+1);
    }

    else
    {
        caminho(s,p[v]);
        printf("  %d  ",v+1);
    }
}

int bellmanFord(int s)
{
    int i,v1,v2;
    init(s);
    for(i=0;i<(n-1);i++)
    {
        for(v1=0;v1<n;v1++){
            for(v2=0;v2<n;v2++){
                if(grafo[v1][v2][0]==1){
                    calculo(v1,v2);
                }
            }
        }
    }
    
    for(i=0;i<n;i++){
        for(v1=0;v1<n;v1++){
            if(grafo[i][v1][0]==1)
            {
                if(d[v1]>(d[i]+grafo[i][v1][1])){
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    int h, f, i, l;
    int v1,v2,c,s,v,w;

    if (argc == 1){
        printf("\nAdicione um texto de entrada! (ex: -f entrada.txt)\n");
        exit(0);
    }

    int opcao;
    while((opcao = getopt(argc, argv, "help:f:")) != -1)
	{
		switch(opcao)
		{
            case 'h':
                printf("\nUse o parâmetro -f para colocar a entrada. (ex: -f entrada.txt)\n");
                return 0;
            case 'f':
                file = fopen(optarg, "r");
                if(file == NULL){
                    printf("TXT vazio.\n");
                    return 0;
                }
                fscanf(file, "%d %d" , &n, &m);

                for(i=0;i<m;i++)
                {
                    fscanf(file, "%d %d %d" , &v1, &v2, &w);
                    v1--;
                    v2--;
                    grafo[v1][v2][0]=1;
                    grafo[v1][v2][1]=w;
                }
                break;
		}
	}

    printf("\nDigite o vértice inicial: ");
    scanf("%d", &s);

    printf("\nDigite o vértice final: ");
    scanf("%d", &v);

    bellmanFord(s-1);
    printf("\nO menor caminho de (%d) para (%d) é: \n\n",s,v);
    
    caminho(s-1,v-1);
    printf("\n\nA distância mínima entre as duas arestas é %d\n",d[v-1]);
    return 0;
}
