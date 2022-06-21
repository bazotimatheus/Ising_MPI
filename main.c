#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../lib/ising.h"

#ifdef MPI
    #include <mpi.h>

    /* Variaveis MPI */
    int mpi_root = 0;
    int my_id, num_nodes;
    int tag = 1, mpi_err;
    MPI_Status rstatus;
#endif // MPI

#ifdef MPI
extern void imprimeParametros(int size);
#else
extern void imprimeParametros();
#endif // MPI

void main(int argc, char *argv[])
{
    FILE *fp;
    int **Spins, **prox_Spins;
    double *mag, *ene, *temperatura, *cal;
    double T, dT;

/* Aloca memoria */
    Spins = (int **)malloc((L+2)*sizeof(int *));
    prox_Spins = (int **)malloc((L+2)*sizeof(int *));
    for(i = 0; i < (L+2); i++)
    {
        Spins[i] = (int *)malloc((L+2)*sizeof(int));
        prox_Spins[i] = (int *)malloc((L+2)*sizeof(int));
    }
    mag = (double *)malloc(nT*sizeof(double));
    ene = (double *)malloc(nT*sizeof(double));
    cal = (double *)malloc(nT*sizeof(double));
    temperatura = (double *)malloc(nT*sizeof(double));

/* Abre arquivo de saida e inicia a semente */
    fp = fopen("data/data.out","a");
    
    srand(time(NULL));

/* Inicia matriz de spins (Verificar arquivo "ising.c") */
    IniciaSpins(Spins);

/* Seta temperatura inicial e tamanho dos passos dT */
    T = T_i;
	dT = (T_f-T_i)/(nT-1);
	
#ifdef MPI
    double *recvT;
    int num_elementos;
    
    for(l = 0; l < nT; l++)
    {
        temperatura[l] = T;
        T += dT;
    }

/* Abre regiao paralela */    
	mpi_err = MPI_Init(&argc, &argv);
    mpi_err = MPI_Comm_size(MPI_COMM_WORLD, &num_nodes );
    mpi_err = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

#ifdef OUTPUT
    time_t begin = time(NULL);
    if(my_id == mpi_root)
    {
        imprimeParametros(num_nodes);
    }
#endif

    num_elementos = nT/num_nodes;

    recvT = (double *)malloc(num_elementos*sizeof(double));
    
    mpi_err = MPI_Scatter(temperatura, num_elementos, MPI_DOUBLE, recvT, num_elementos, MPI_DOUBLE, mpi_root, MPI_COMM_WORLD);

/* Cada processo executa os passos em T.
 * Executa a rotina de Monte Carlo com algoritmo de Metropolis 
 * e salva quantidades de interesse em um arquivo de saida. */
	for(l = 0; l < num_elementos; l++)
    {
		MonteCarlo(Spins, prox_Spins, mag, ene, cal, recvT[l], l);
        fprintf(fp, "%lf\t%lf\t%lf\t%lf\n", recvT[l], mag[l], ene[l], cal[l]);
	}

#ifdef OUTPUT
	if (my_id == mpi_root)
    {
        time_t end = time(NULL);
	    printf("The elapsed time is %d seconds.\n", (end - begin));
    }
#endif

    mpi_err = MPI_Finalize();
#else

#ifdef OUTPUT
    /* Imprime os parametros do programa */
    imprimeParametros();
    time_t begin = time(NULL);
#endif

/* Executa os passo em T.
 * Executa a rotina de Monte Carlo com algoritmo de Metropolis 
 * e salva quantidades de interesse em um arquivo de saida. */
	for(l = 0; l < nT; l++)
    {
		MonteCarlo(Spins, prox_Spins, mag, ene, cal, T, l);
        fprintf(fp, "%lf\t%lf\t%lf\t%lf\n", T, mag[l], ene[l], cal[l]);
        T += dT;
	}

    #ifdef OUTPUT
        time_t end = time(NULL);
        printf("\nThe elapsed time is %d seconds.\n", (end - begin));
    #endif // OUTPUT
#endif /* Fim da execucao do programa */ 

/* Libera espaços de memoria */
	for(i = 0; i <= (L+1); i++) free(Spins[i]);
	for(i = 0; i <= (L+1); i++) free(prox_Spins[i]);
	free(Spins);
    free(prox_Spins);
    free(mag);
    free(ene);
    free(cal);
    free(temperatura);

/* Fecha arquivo de saida */
	fclose(fp);
}

#ifdef MPI
void imprimeParametros(int size)
{
    printf("\nRodando o modelo de Ising em paralelo com os seguintes parametros:\n");
    printf("\n Numero de processos = %d", size);
#else
void imprimeParametros()
{
    printf("\nRodando o modelo de Ising em serial com os seguintes parametros:\n");
#endif
    printf("\n Tamanho da malha = %d", L);
    printf("\n Numero de passos sobre a temperatura = %d", nT);
    printf("\n Numero de passos de Monte Carlo para cada T = %d",nMC);
    printf("\n Quantidade de spins pra baixo (S_up + S_down = 1) = %.2f", S_down);
    printf("\n Temperatura inicial = %.2f", T_i);
    printf("\n Temperatura final = %.2f\n", T_f);
    printf("\n");
}
