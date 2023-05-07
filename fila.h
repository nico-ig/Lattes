// Feito por Nico Iancoski GRR20210574

#ifndef _FILA_
#define _FILA_

typedef struct fila t_f;
typedef struct publicacao t_pub;

// ********************************* Funcoes ******************************* //

// Cria um fila e retorna um ponteiro para ela,
// em caso de erro retorna NULL
t_f *criaFila();

// Retorna o tamanho de uma fila
int tamFila(t_f *f);

// Insere um elemento na fila, retorna 1 em caso de sucesso
// e 0 em caso de erro
int insereFila(t_f *f, t_pub *pub);

// Retira o primeiro elemento da fila, retorna 1 em caso de sucesso
// e 0 caso contrario
int retiraFila(t_f *f, t_pub **pub);

#endif
