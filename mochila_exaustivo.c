#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define NOME_ARQ "sac94\\PET4.DAT"

///**********VARIAVEIS GLOBAIS IMPORTANTES**********///
FILE* arq;

int n_mochilas; //Quantidade de restrições no problema
int n_objetos;  //Quantidade de objetos
int* vet;
int* mochilas;  //Vetor que contém o peso maximo de todas as mochilas, cada mochila é como se fosse uma dimensão
int* objetos;   //Vetor que contém o valor de todos os objetos, a instancia de 0 e 1 ainda n existe
int** restr;    //Matriz que contém o peso dos objetos: restr[n_mochila][n_objeto]

///************************************************///

int* le_dados(int max) //Essa função le os dados, é melhor do que ficar escrevendo isso toda hora
{
    int i;
    int* vet = (int*)malloc(max*sizeof(int));
    for(i=0;i<max;i++)
    {
        fscanf(arq,"%d",&vet[i]);
        printf("[%d]",vet[i]);
    }
    printf("\n");
    return vet;
}

int checkfit(int* individuo)//No momento é só um somatório do valor dos itens
{
    int fit = 0,i,j;
    int* check_restr = (int*)malloc(n_mochilas*sizeof(int));//Cada indice checa se o individuo ja passou da restricao
    memset(check_restr,0,n_mochilas*sizeof(int));//Poe zero em tudo
    for(i=0;i<n_objetos;i++)
    {
        fit += objetos[i]*individuo[i];
        for(j=0;j<n_mochilas;j++)//Checa todas as restriçoes e depois faz alguma coisa
        {
            check_restr[j] += restr[j][i]*individuo[i];
            if(check_restr[j] > mochilas[j]) //Estourou uma restrição, tem que mudar algo de 1 pra 0
            {
                free(check_restr);
                return -1;
            }
        }


    }
    free(check_restr);
    return fit;
}

void* bt(int* vet,int pos,int* melhor)
{
    if(pos == n_objetos)
    {
        int aux;
        aux = checkfit(vet);
        if(aux > *melhor)
            *melhor = aux;
    }
    else
    {
        int i;
        for(i=0;i<2;i++)
        {
            vet[pos] = i;
            bt(vet,pos+1,melhor);
        }
    }

}

int main(int argc, char *argv[])
{
    ///PARTE 1 - Inicialização
    clock_t inicio = clock();
    double tempo;
    int i = 0;
    int n = 0;
    int melhor = 0;
    int aux = 0;

    arq = fopen(argv[1],"r");
    fscanf(arq,"%d",&n_mochilas);
    fscanf(arq,"%d",&n_objetos);
    printf("Mochilas:%d Objetos: %d\n",n_mochilas,n_objetos);

    printf("Peso/valor dos objetos:\n");
    objetos = le_dados(n_objetos);

    printf("Capacidades/restricoes das mochilas:\n");
    mochilas = le_dados(n_mochilas);

    printf("\nRestricoes dos objetos:\n");
    restr = (int**)malloc(n_mochilas*sizeof(int*));
    for(i=0;i<n_mochilas;i++)
    {
        printf("Restricoes #%d:\n",i+1);
        restr[i] = le_dados(n_objetos);
    }

    vet = (int*)malloc(n_objetos*sizeof(int));
    memset((void*)vet,0,n_objetos);

    bt(vet,0,&melhor);
    tempo = (double)(clock() - inicio)/CLOCKS_PER_SEC;
    printf("Resposta: %d em %.3f\n",melhor,tempo);
}
