// Extraido e adaptado de:
// de https://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C

#ifndef _EDITDISTANCE_
#define _EDITDISTANCE_

// Macro para o limitante da distancia de edicao
#define LIM(s1, s2) ( (s1) < (s2) ? ((int)(s1)/4) : ((int)(s2)/4) )

// Transforma a funcao editDistance em uma funcao booleana,
// retorna 0 para sim e 1 para nao
int editDistanceBool(const char *s1, const char *s2);

// Verifica se uma string eh substring de outra, utilizando o criterio
// da distancia de edicao. Retorna 0 caso seja e 1 caso contrario
int editDistanceStr(char *str_buf, char *str_p);

// Eh preciso chamar essa funcao para liberar a memoria 
// utilizada pela funcao editDistance
void liberaEditDistance();

#endif

