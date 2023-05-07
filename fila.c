// Feito por Nico Iancoski GRR20210574

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fila.h"

// ********************************* Structs ******************************* //

// Representa um nodo da fila
typedef struct nodo
{
  t_pub *pub;
  struct nodo *prox;

} t_nodo;

// Representa a fila
typedef struct fila
{
  t_nodo *ini;
  t_nodo *fim;
  int tam;

} t_f;

// ********************************* Funcoes ******************************* //

// Cria uma fila e retorna um ponteiro para ela, em caso de erro retorna NULL
t_f *criaFila()
{
  t_f *f;

  if ( ! (f = malloc(sizeof(t_f))) ) { return NULL; }

  f->ini = NULL;
  f->fim = NULL;
  f->tam = 0;

  return f;
}

// Verifica se uma fila esta vazia, retorna 1 caso esteja e 0 caso contrario
int filaVazia(t_f *f) { return !(f->ini); }

// Retorna o tamanho de uma fila
int tamFila(t_f *f) { return f->tam; }

// Insere um elemento na fila, retorna 1 em caso de sucesso e 0 em caso de erro
int insereFila(t_f *f, t_pub *pub)
{
  t_nodo *nodo;
  if ( ! (nodo = malloc(sizeof(t_nodo))) ) { return 0; }

  nodo->pub = pub;
  nodo->prox = NULL;

  if ( filaVazia(f) ) { f->ini = nodo; }
  else { f->fim->prox = nodo; }

  f->fim = nodo;
  f->tam++;
  
  return 1;
}

// Retira um elemento da fila, retorna 1 em caso de sucesso e 0 caso contrario
int retiraFila(t_f *f, t_pub **pub)
{
  if ( filaVazia(f) ) { return 0; }
  
  t_nodo *r = f->ini;

  *pub = r->pub;
  f->ini = r->prox;
  f->tam--;

  free(r);
  return 1;
}

