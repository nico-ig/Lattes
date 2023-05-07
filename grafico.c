// Feito por Nico Iancoski GRR20210574
#define _GNU_SOURCE

#include <locale.h>

#include "math.h"
#include "utils.h"
#include "grafico.h"
#include "interface.h"

#define RED  "\x1B[31m"
#define BLU  "\x1B[34m"
#define DFT  "\x1B[0m"

// Vetor de cores
char *cor[] = { DFT, RED, BLU };

// Imprime a barra de um dado de acordo com a escala
void imprimeBarra(int tam, int e)
{
  for ( int cnt = 0; cnt < tam; cnt+=e ) { printf("#"); }
}

// Imprime o cabecalho
void cabecalhoGrafico(int tam_l1, char *titulo, int tam_l2, int max_y, int e)
{
  printf("%*s%s\n", tam_l1, "Titulo: ", titulo);
  imprimeLinha(tam_l2 + (max_y + 1)/e);
  printf("\n");
}

// Calcula a escala para o grafico
int calculaEscala(int max_y, int max_label)
{
  char *str_max_y;
  if ( asprintf(&str_max_y, "%d", max_y) == -1 ) { return 0; }
  int len_max_y = (int)strlen(str_max_y);
  free(str_max_y);

  int tam_max = 78-max_label-len_max_y;
  
  if ( max_y <= tam_max ) { return 1; }
  else { return ceil(max_y/(float)tam_max); }
}

// Imprime um grafico com a qtd de pub em periodicos e eventos
void plotGrafico(int **val, int l, int c, char **la_l, 
                 char **la_c, char *tit, int max_y)
{
  // Calcula o tamanho do maior label
  int max_l = calcMaxStr(la_l, l);
  int max_c = calcMaxStr(la_c, c);

  // Calcula o maior valor para uma barra
  //int max_y = calcMaxMatriz(val, l, c);

  int e = calculaEscala(max_y, max_c);

  cabecalhoGrafico(max_l+1, tit, max_c+6, max_y, e);
  
  // Imprime as barras para cada linha da matriz
  for ( int i = 0; i < l; i++ )
  {
    // Imprime o label referente ao agrupamento
    printf("%*s", max_l, la_l[i]);
    printf("%*s%s (1:%d)\n", max_l, " ", "Tipo/Quantidade", e);
    imprimeLinha(max_c+6 + (max_y + 1)/e); 

    for ( int j = 0; j < c; j++ )
    {
      // Imprime o label referente a cada dado do agurpamento
      printf("%*s|", max_c, la_c[j]);

      printf("%s", cor[i+1]);
      imprimeBarra(val[i][j], e);
      printf("%s", cor[0]);
    
      printf(" %d\n", val[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
