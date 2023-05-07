// Extraido de e adaptado de:
// https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C

#include "utils.h"
#include "edit-distance.h"

#define MIN3(a, b, c) ((a)<(b) ? ((a)<(c) ? (a) : (c)) : ((b)<(c) ? (b) : (c)))

// Variaveis globais para evitar alocar toda vez que a funcao 
// edit distance for chamada
char* col = NULL;
int allocated_col = 0;

// Funcao para calcular quantas edicoes sao necessarias entre
// duas strings para que elas fiquem iguais
int editDistance (char *s1, char *s2) {
  int s1len, s2len, olddiag;
  s1len = strlen(s1);
  s2len = strlen(s2);

  // Realloca a coluna somente se necessario
  if (allocated_col < s1len+1) {
    col = realloc(col, sizeof(char) * (s1len+1));
    allocated_col = s1len+1;
  }

  // Poda
  if (s1len >= s2len + LIM(s1len, s2len)+1) { return LIM(s1len, s2len)+1; }
  if (s2len >= s1len + LIM(s1len, s2len)+1) { return LIM(s1len, s2len)+1; }

  for (int y = 1; y <= s1len; y++) {
    col[y] = y;
  }

  for (int x = 1; x <= s2len; x++) {
    int mn = x;
    col[0] = x;
    int lastdiag = x-1;
    for (int y = 1; y <= s1len; y++) {
      olddiag = col[y];
      col[y] = MIN3(col[y]+1, col[y-1]+1, lastdiag+(s1[y-1] == s2[x-1] ? 0:1));
      if (col[y] < mn) { mn = col[y]; }
      lastdiag = olddiag;
    }
    if (mn >= LIM(s1len, s2len) + 1) { return LIM(s1len, s2len) + 1; }
  }

  return col[s1len];
}

// Transforma a funcao editDistance em uma funcao booleana,
// retorna 0 para sim e 1 para nao
int editDistanceBool(const char *s1, const char *s2)
{
  if ( editDistance((char *)s1, (char *)s2) <= LIM(strlen(s1), strlen(s2)) ) 
    return 0;
  
  return 1;
}

// Verifica se uma string eh substring de outra, utilizando o criterio
// da distancia de edicao. Retorna 0 caso seja e 1 caso contrario
int editDistanceStr(char *str_buf, char *str_p)
{
  char *str, *str_min;
  char *str_p_min = myToLower(str_p);

  while ( sscanf(str_buf, " %m[^\n]", &str) != EOF )
  {
    str_min = myToLower(str); 
    if ( !editDistanceBool(str_p_min, str_min) ) 
    {
      myFree(3, str, str_p_min, str_min); 
      return 0; 
    }
    
    str_buf += strlen(str);
    myFree(2, str, str_min);
  }

  free(str_p_min);
  return 1;
}

// Eh preciso chamar essa funcao para liberar a memoria 
// utilizada pela funcao editDistance
void liberaEditDistance() { free(col); }


