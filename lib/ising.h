#ifndef ising_h__
#define ising_h__

/* Variáveis do modelo de Ising */
#define L 10		/* Tamanho da malha */
#define nT 384		/* Número de passos sobre a temperatura */
#define nMC 1024	/* Número de passos de Monte Carlo para cada T */
#define S_down 0.1	/* Quantidade de spins pra baixo (S_up + S_down = 1) */
#define T_i 1.0		/* Temperatura inicial */
#define T_f 5.0		/* Temperatura final */

/* Variáveis auxiliares */
/* Respectivamente: linhas, colunas, iterações de Monte Carlo e iterações em T */
int i, j, k, l; 

extern double NumeroAleatorio();
extern int VizinhosProximos(int **S, int lin, int col);
extern void IniciaSpins(int **S);
extern void CalculaMagnetizacaoEnergia(int **S, double *m, double *e);
extern void CalorEspecifico(double *calor, int l, double energia, double energia_quad, double temperatura);
extern void Metropolis(int **S, int lin, int col, double *m, double *e, double E_flip, double temperatura);
extern void MonteCarlo(int **S, int **prox_S, double *m, double *e, double *calor, double temperatura, int l);

#endif // ising_h__