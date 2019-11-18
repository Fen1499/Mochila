#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#include <time.h>
#define PPL 100
#define niterate 1000
#define nelite 5
#define NOME_ARQ "sac94\\WEISH02.DAT"

typedef struct cromo
{
    int* vet;
    int fitness;
}Cromo;

///**********VARIAVEIS GLOBAIS IMPORTANTES**********///
FILE* arq;

int n_mochilas; //Quantidade de restrições no problema
int n_objetos;  //Quantidade de objetos
int v_opt;      //Valor ótimo lido do arquivo
int v_max=0;    //Somatório do valor de todos os objetos da instancia
int* mochilas;  //Vetor que contém o peso maximo de todas as mochilas, cada mochila é como se fosse uma dimensão
int* objetos;   //Vetor que contém o valor de todos os objetos, a instancia de 0 e 1 ainda n existe
int** restr;    //Matriz que contém o peso dos objetos: restr[n_mochila][n_objeto]
Cromo* ppl;     //Vetor de struct do cromossomo Cromo.vet acessa o individuo.
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

int pertence(int* vet,int tam,int val)
{
    int i;
    for(i=0;i<tam;i++)
    {
        if(vet[i] == val)
            return 1;
    }
    return 0;
}

int* get_elite()//Retorna os N melhores individuos da população, essa função ficou meio pesada
{
    int elite[nelite];
    memset((void*)elite,0,sizeof(int)*nelite);
    int* elitepos = (int*)malloc(nelite*sizeof(int));
    memset((void*)elitepos,-1,sizeof(int)*nelite);
    int i,j;
    for(i=0;i<nelite;i++)
    {
        for(j=0;j<PPL;j++)
        {
            if(ppl[j].fitness > elite[i] && !pertence(elitepos,nelite,j))
            {
                elite[i] = ppl[j].fitness;
                elitepos[i] = j;
            }
        }
    }
    return elitepos;
}

int* gera_cromo()//Essa função gera um individuo aleatório
{
    int i=0;
    int* individuo = (int*)malloc(n_objetos*sizeof(int));
    for(i=0;i<n_objetos;i++)
    {
        individuo[i] = rand()%2;
    }
    return individuo;
}

int* check_valido(int* individuo,int* fit)//Essa função checa se a solução é valida, se n for ela muda ate ficar
{
    int* re_individuo = individuo;
    int pos;
    {
        *fit = checkfit(individuo);
        while(*fit < 0)
        {
            pos = rand()%n_objetos;
            re_individuo[pos] = 0;
            *fit = checkfit(re_individuo);
        }
    }
    return re_individuo;
}

Cromo trymute(Cromo c)
{
    int pos = rand()%n_objetos;
    if(rand()%100 < 25)
    {
        c.vet[pos] = 1 - c.vet[pos]; //X = ¬X
        check_valido(c.vet,&c.fitness);
    }
    return c;
}

Cromo crossover(int* cromo1,int* cromo2)//Cria um novo individuo que é sempre valido a partir de dois outros e calcula seu fitness
{
    int i;
    int corte = 1+rand()%n_objetos;
    Cromo newborn1;
    Cromo newborn2;
    newborn1.vet = (int*)malloc(n_objetos*sizeof(int));
    newborn2.vet = (int*)malloc(n_objetos*sizeof(int));
    for(i=0;i<n_objetos;i++)
    {
        if(i<(n_objetos/corte))
        {
            newborn1.vet[i] = cromo1[i];
            newborn2.vet[i] = cromo2[i];
        }
        else
        {
            newborn1.vet[i] = cromo2[i];
            newborn2.vet[i] = cromo1[i];
        }
    }
    newborn1.vet = check_valido(newborn1.vet,&newborn1.fitness);
    newborn2.vet = check_valido(newborn2.vet,&newborn2.fitness);

    if(newborn1.fitness > newborn2.fitness)
    //if(rand()%2 > 0)
        return trymute(newborn1);
    else
        return trymute(newborn2);

}


Cromo* gera_ppl()//Essa função só serve pra alocar a matriz e chamar a outra função.Ficou bem mais facil assim
{
    int i=0;
    int fit; //Isso aqui só serve pra função retornar 2 valores.
    Cromo* ppl = (Cromo*)malloc(PPL*sizeof(Cromo));
    for(i=0;i<PPL;i++)
    {
        ppl[i].vet = gera_cromo();
        ppl[i].vet = check_valido(ppl[i].vet,&fit);//checa se a solução é valida
        ppl[i].fitness = fit;
        printf("%d: %d\n",i,ppl[i].fitness);
    }
    printf("\n");
    return ppl;
}

void print_ind(int* vet)
{
    int i;
    for(i=0;i<n_objetos;i++)
    {
        printf("%d ",vet[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    ///PARTE 1 - Inicialização
    srand(time(NULL));
    int i=0;
    int n=0;
    int bestboy=0;

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

    fscanf(arq,"%d",&v_opt);
    printf("valor_opt:%d\n",v_opt); //Eu sei que ta tudo certo porque esse valor é o ultimo que tem que ser lido e ele ta certo

    for(i=0;i<n_objetos;i++) //Inicialização do valor maximo
    {
        v_max += objetos[i];
    }
    printf("valor_max:%d\n",v_max);

    ///PARTE 2 - Geração da população
    ppl = gera_ppl();//Gera uma população inicial aleatória
    Cromo* ppl_novo = (Cromo*)malloc(PPL*sizeof(Cromo));

    ///PARTE 3 - Crossover e Mutação
    int* elite;

    //Aqui ja da pra por um loop pra n iterações//
    for(n=0;n<niterate;n++)
    {
        elite = get_elite();//RETORNA A POSIÇÃO NA POPULAÇÃO

        for(i=0;i<PPL;i++)
        {
            if(rand()%4 > 2)
                //ppl_novo[i] = crossover(ppl[elite[1]].vet,ppl[rand()%PPL].vet);
                ppl_novo[i] = crossover(ppl[rand()%PPL].vet,ppl[rand()%PPL].vet);
                //ppl_novo[i] = crossover(ppl[elite[rand()%nelite]].vet,ppl[elite[rand()%nelite]].vet);
            else
                //ppl_novo[i] = crossover(ppl[elite[rand()%nelite]].vet,ppl[rand()%PPL].vet);
                //ppl_novo[i] = crossover(ppl[elite[0]].vet,ppl[rand()%PPL].vet);

            ppl_novo[i] = crossover(ppl[elite[rand()%nelite]].vet,ppl[rand()%PPL].vet);

        }
        printf("Iteracao: %d\n",n);
        for(i=0;i<nelite;i++)
        {
            printf("Elite %d: %d\n",elite[i],ppl[elite[i]].fitness);
            //print_ind(ppl[elite[i]].vet);
        }
        printf("\n");
        ppl = ppl_novo;

    }
    //printf("Melhores: %d %d %d %d %d\n",ppl[elite[0]].fitness,ppl[elite[1]].fitness,ppl[elite[2]].fitness,ppl[elite[3]].fitness,ppl[elite[4]].fitness);
    for(i=0;i<nelite;i++)
    {
        if(ppl[elite[i]].fitness > bestboy)
        {
            bestboy = ppl[elite[i]].fitness;
        }
    }
    printf("Valor_opt: %d",v_opt);
    //Fim da iteracao

    return bestboy;
}
