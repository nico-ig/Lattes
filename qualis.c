// Feito por Nico Iancoski GRR20210575

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"
#include "qualis.h"

// Niveis de publicacoes na Qualis
char *estratos[C_NIVEL] = {"A1", "A2", "A3", "A4", "B1", 
                           "B2", "B3", "B4", "C", "NC"};

// ********************************* Funcoes ******************************* //

// Retorna um numero que representa um nivel Qualis
int nivelQualis(char *nivel)
{
  for (int i = 0; i < C_NIVEL; i++)
    if ( ! strcmp(estratos[i], nivel) ) { return i; }
  
  return -1;
}

// Cria um novo elemento to tipo qualis
t_qualis insereQualis(char *nome, char *nivel)
{
  t_qualis q;
  
  q.nome = nome;
  q.nivel = nivel;

  return q;
}

// Separa uma linha de um arquivo qualis em nome e nivel
// Retorna a saida da funcao de insercao
t_qualis leLinhaQualis(char *str)
{
  char *ptr, *nome, *nivel;

  ptr = strrchr(str, ' ');
  nome = strndup(str, strlen(str) - strlen(ptr));
  nivel = strdup(ptr+1);

  free(str);
  return insereQualis(nome, nivel);
}

// Le todas as linhas de uma lista qualis, alocando em um vetor de
// struct qualis, que possui os campos nome e nivel
// Em caso de erro retorna NULL e seta em 1 o codigo de erro,
// em caso de sucesso retorna um ponteiro para a struct
t_qualis *leQualis(char *arq_nome, int *arq_cnt)
{
  // Ve o tamanho do arquivo, para fazer a alocacao do vetor
  struct stat arq_s;
  if ( stat(arq_nome, &arq_s) == -1 ) { ERR; }
  
  t_qualis *v; 
  int arq_tam = arq_s.st_size;
  if ( ! (v = malloc(sizeof(t_qualis) * arq_tam)) ) { ERR; } 

  FILE *arq;
  if ( ! (arq = fopen(arq_nome, "r")) ) { ERR; }

  int i; char *str;
  for ( i = 0; fscanf(arq, "%m[^\n]%*c", &str) != EOF; i++ )
    v[i] = leLinhaQualis(str);

  *arq_cnt = i;

  fclose(arq);
  return v;
}
