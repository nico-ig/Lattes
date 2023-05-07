// Feito por Nico Iancoski GRR20210574

#ifndef _INTERFACE_
#define _INTERFACE_

typedef struct lattes t_lattes;

// Imprime uma linha de tamanho "tam"
void imprimeLinha(int tam);

// Imprime o menu e gerencia a chamada das opcoes
void menu(t_lattes *lattes);

// Imprime o relatorio com as informacoes lidas
void relatorio(t_lattes *lattes);

#endif
