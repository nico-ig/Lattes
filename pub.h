// Feito por Nico Iancoski GRR20210574

#ifndef _PUB_
#define _PUB_

#include "fila.h"
#include "qualis.h"
#include "estruturas.h"

extern char *v_labels[C_TIPOS];

// Tipos de publicacoes
typedef enum tipos_de_publicacoes {per, conf} e_tipo;

// ****************************** Publicacoes ****************************** //

// Soma todos os elementos em um nivel
int somaNivel (t_dado *v, int cnt);

// Soma todos os elementos de um tipo de publicacao
int somaPub(t_dado **v_pub, int *v_cnt);

// Le todas as publicacacoes, tanto de periodicos como 
// de conferencias, e retorna um ponteiro para uma fila 
// contendo as publicacoes do um tipo
t_f *lePub(char *str, e_tipo t, char* autor, t_f *f);

// ********************************* Dados ********************************* //

// Utiliza os valores de uma lista qualis para classificar as publicacoes em 
// uma fila de publicacao e preencher os dadoes referentes a elas em um grupo. 
// Retorna um ponteiro para o grupo e, em caso de falha, retorna NULL
t_grupo *dadosGrupo(t_f *f_pub, t_qualis *v_q, int cnt_q, t_grupo *g, e_tipo t);

#endif
