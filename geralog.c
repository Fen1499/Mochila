#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define TRIES 20

int main()
{
    int escolha = 1;
    int i;
    int bestboy = 0;
    int aux = 0;
    double tempo = 0;
    char filename[8];
    memset((void *) filename,(int) NULL,8);
    char path[256];
    memset((void *) path,(int) NULL,256);
    FILE* arqin;
    FILE* arqout;

    system("cd C:\\Users\\Fna_\\OneDrive\\Documentos\\C\\knapsack");
    if(escolha == 1)///GERA LOG DO GENETICO
    {
        arqin = fopen("instancias_exaustivo.txt","r");
        arqout = fopen("log.txt","w");
        while(!feof(arqin))
        {
        fscanf(arqin,"%s",filename);
        sprintf(path,"knapsack.exe sac94\\%s.DAT",filename);
        for(i=0;i<TRIES;i++)
        {
            clock_t inicio = clock();
            bestboy += system(path);
            tempo += (double)(clock() - inicio)/CLOCKS_PER_SEC;
            //if(aux > bestboy)bestboy = aux;
        }
        tempo = tempo/TRIES;
        fprintf(arqout,"%s: %d %.3f\n",filename,bestboy,tempo);
        bestboy = 0;
        aux = 0;
        tempo = 0;
        }
    }
    if(escolha == 2)///GERA LOG DO EXAUSTIVO
    {
        arqin = fopen("instancias_exaustivo.txt","r");
        arqout = fopen("log_exaustivo.txt","w+");

        while(!feof(arqin))
        {
            fscanf(arqin,"%s",filename);
            sprintf(path,"mochila_exaustivo.exe sac94\\%s.DAT",filename);
            clock_t inicio = clock();
            system(path);
            tempo = (double)(clock() - inicio)/CLOCKS_PER_SEC;
            fprintf(arqout,"%s: %.3f\n",filename,tempo);
        }
    }
    fclose(arqin);
    fclose(arqout);
    return 0;
}

