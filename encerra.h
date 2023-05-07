// Feito por Nico Iancoski GRR20210574

#ifndef _ENCERRA_
#define _ENCERRA_

typedef struct lattes t_lattes;
typedef struct dado t_dado;

// ******************************** Vetores ******************************** //

// Libera toda a memoria utilizada por uma matriz
void liberaMatriz(int **m, int cnt);

// Libera toda a memoria utilizada por um vetor de strings
void liberaVStr(char **v_str, int cnt);

// ****************************** Estruturas ******************************* //

// Libera toda a memoria utilizada pelo vetor de anos
void liberaDado(t_dado *v_dado, int ini, int fim);

// Libera toda a memoria em uso ao final da execucao
void encerra(t_lattes *lattes);

#endif
