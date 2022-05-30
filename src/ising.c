#include <stdlib.h>
#include <math.h>

#include "../lib/ising.h"

/* Gera numero aleatorio no intervalo [0,1] */
double NumeroAleatorio() {
    return (double)rand() / (double)( (unsigned)RAND_MAX + 1);
}

/* Soma os spins dos vizinhos mais proximos */
int VizinhosProximos(int **S, int lin, int col) {
    return S[lin+1][col] + S[lin-1][col] + S[lin][col+1] + S[lin][col-1];
}

/* Inicia a matriz de spins com condicões de contorno periodicas */
void IniciaSpins(int **S) {
    for(i = 1; i <= L; i++) for(j = 1; j <= L; j++) {
        if(S_down <= NumeroAleatorio()) S[i][j] = 1;
        else S[i][j] = -1;
    }

/* Matriz estendida */
    for(i = 1; i <= L; i++) {
        S[i][0] = S[i][L];
        S[i][L+1] = S[i][1];
        S[0][i] = S[L][i];
        S[L+1][i] = S[1][i];
    }

/* Os cantos sao vazios */
    S[0][0] = 0;
    S[0][L+1] = 0;
    S[L+1][0] = 0;
    S[L+1][L+1] = 0;
}

/* Calcula a magnetizacao e energia */
void CalculaMagnetizacaoEnergia(int **S, double *m, double *e) {
    *m = 0;
    *e = 0;
    for(i = 1; i <= L; i++) for(j = 1; j <= L; j++) {
        *m += S[i][j];
        *e = (-1)*(S[i][j]*VizinhosProximos(S,i,j)*0.5);
    }
}

/* Calcula o calor especifico */
void CalorEspecifico(double *calor, int l, double energia, double energia_quad, double temperatura) {
    double x;

    x = energia_quad/nMC - (energia/nMC)*(energia/nMC);

    calor[l] = x / (temperatura*temperatura*L*L);
}

/* Algoritmo de Metropolis (Fonte: Computational Physics, 2a edicao, Nicholas J. Giordano, secao 8.4, exemplo 8.1) */
void Metropolis(int **S, int lin, int col, double *m, double *e, double E_flip, double temperatura) {
    if(E_flip <= 0) {
        S[lin][col] = -S[lin][col]; /* Flipa o spin */
        *m += S[lin][col] * 2.0;
        *e -= E_flip;
    }
    else {
        if(NumeroAleatorio() < exp((-1) * E_flip / temperatura)) {
            S[lin][col] = -S[lin][col]; /* Flipa o spin */
            *m += S[lin][col] * 2.0;
            *e += E_flip;
        }
    }
}

/* Rotina de Monte Carlo com algoritmo de Metropolis */
void MonteCarlo(int **S, int **prox_S, double *m, double *e, double *calor, double temperatura, int l) {
    double m_atual, soma_m = 0;
    double e_atual, soma_e = 0, soma_e_quad = 0;
    double E_flip;
    
    for(k = 0; k < nMC; k++) {
/* Calcula magnetizacao e energia para a matriz de spins */
        CalculaMagnetizacaoEnergia(S, &m_atual, &e_atual);

/* Atualiza a proxima matriz de spins */
        for(i = 0; i <= L+1; i++)
            for(j = 0; j <= L+1; j++)
                prox_S[i][j] = S[i][j];

/* Metropolis na primeira linha */
        for(j = 1; j <= L; j++) {
            E_flip = prox_S[1][j] * VizinhosProximos(prox_S, 1, j) * 2.0;
            Metropolis(prox_S, 1, j, &m_atual, &e_atual, E_flip, temperatura);
        }

/* Ajusta matriz estendida */
        for(j = 1; j <= L; j++) {
            prox_S[0][j] = prox_S[L][j];
            prox_S[L+1][j] = prox_S[1][j];
        }

/* Metropolis no miolo da malha */
        for(i = 2; i < L; i++) {
            for(j = 1; j <= L; j++) {
                E_flip = prox_S[i][j] * VizinhosProximos(prox_S, i, j) * 2.0;
                Metropolis(prox_S, i, j, &m_atual, &e_atual, E_flip, temperatura);
            }
/* Ajusta matriz estendida */
            prox_S[i][0] = prox_S[i][L];
            prox_S[i][L+1] = prox_S[i][1];
        }

/* Metropolis na ultima linha */
        for(j = 1; j <= L; j++) {
            E_flip = prox_S[L][j] * VizinhosProximos(prox_S, i, j) * 2.0;
            Metropolis(prox_S, i, j, &m_atual, &e_atual, E_flip, temperatura);
        }

/* Ajusta matriz estendida */
        for(j = 1; j <= L; j++) {
            prox_S[L+1][j] = prox_S[1][j];
            prox_S[0][j] = prox_S[L][j];
        }

/* Soma magnetizacao, energia e o quadrado da energia */
        soma_m += m_atual;
        soma_e += e_atual;
        soma_e_quad += e_atual * e_atual;
    }
/* Fim do loop em k */
    
/* Atualiza a matriz de spins */
    for(i = 0; i <= L+1; i++)
        for(j = 0; j <= L+1; j++)
            S[i][j] = prox_S[i][j];

/* Calcula o calor especifico */
    CalorEspecifico(calor, l, soma_e, soma_e_quad, temperatura);

/* Calcula as medias de magnetizacao e energia */
    m[l] = soma_m / (double)(nMC*L*L);
    e[l] = soma_e / (double)(nMC*L*L);
}
