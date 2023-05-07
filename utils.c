// Feito por Nico Iancoski GRR20210574

#include <stdarg.h>
#include <ctype.h>

#include "utils.h"

int e_dados = 0;

// ********************************* Strings ******************************* //

// Retorna um ponteiro para uma string que comeca no final da tag
char *strstrP(char *str, char *tag)
{
  return strstr(str, tag) + strlen(tag);
}

// Concatena um titulo em uma string
char *myStrcat(char *str1, char *str2)
{
  int tam = strlen(str1) + strlen(str2) + 2;
  
  if ( ! (str1 = realloc(str1, tam)) ) { return NULL; }
  
  str1 = strcat(str1, "\n");
  str1 = strcat(str1, str2);

  return str1;
}

// Converte uma str para minuscula
char *myToLower(char *str)
{
  char *str_cpy = strdup(str);

  for ( int i = 0; i < (int)strlen(str_cpy); i++ )
    str_cpy[i] = tolower(str[i]);
  
  return str_cpy;
}

// Inicia uma string com o primeiro valor em 0
char *iniciaStr()
{
  char *str;
  if ( ! (str = malloc(sizeof(char))) ) { ERR; }
  str[0] = '\0';;

  return str;
}

// Retorna o valor de uma tag
char *tagValor(char *str, char *tag)
{
  char *ini = strstrP(str, tag) + 2;
  char *fim = strchr(ini, '"');
        
  return strndup(ini, fim-ini);
}

// Cria o nome do arquivo
char *nomeArq(char *nome_dir, char *nome_arq)
{
  int tam = strlen(nome_dir) + strlen(nome_arq) + 4;
  
  char *arq_nome;
  if ( ! (arq_nome = malloc(sizeof(char) * tam)) ) { return NULL; }
  
  sprintf(arq_nome, "./%s/%s", nome_dir, nome_arq);
  
  return arq_nome;
}

// ********************************* Vetores ******************************* //

// Retorna o tamanho da maior string no vetor
int calcMaxStr(char **v_str, int cnt)
{
  int max = strlen(v_str[0]);
  for ( int i = 1; i < cnt; i++ )
    if ( max < (int)strlen(v_str[i]) ) { max = strlen(v_str[i]); }

  return max;
}

// Retorna o maior valor de um vetor
int calcMaxVetor(int *v, int cnt)
{
  int max = v[0];
  for ( int i = 1; i < cnt; i++ )
    if ( max < v[i] ) { max = v[i]; }

  return max;
}

// Retorna o maior valor em uma matriz
int calcMaxMatriz(int **m, int l, int c)
{
  int maxV;
  int max = 0;
  
  for ( int i = 0; i < l; i++ )
  {
    maxV = calcMaxVetor(m[i], c);
    if ( max < maxV ) { max = maxV; }
  }

  return max;
}

// Inicia um vetor com "tam" elementos em 0
int *iniciaVetor(int tam)
{
  int *v;
  if ( ! (v = calloc(tam, sizeof(int))) ) { ERR; }
  return v;
}

// Inicia uma matriz com l*c elementos em 0
int **iniciaMatriz(int l, int c)
{
  int **m;
  if ( ! (m = myMalloc(sizeof(int *), l)) && (l > 0) ) { ERR; }
  for ( int i = 0; i < l; i++ )
      if ( ! (m[i] = iniciaVetor(c)) ) { ERR; }
  return m;
}

// ********************************* Memoria ******************************* //

// Retorna NULL se cnt for 0, se não, retorna
// um ponteiro
void *myMalloc(size_t size, int cnt)
{
  if ( cnt == 0 ) return NULL;
  return malloc(size*cnt);
}

// Libera a memoria de um tanto variavel de parametros
void myFree(int n, ...)
{
  void *del;
  va_list ap;

  va_start(ap, n);
  while ( n-- ) 
  {
    del = va_arg(ap, void *);
    free(del); 
  }
 
  va_end(ap);
}



