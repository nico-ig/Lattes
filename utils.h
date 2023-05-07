// Feito por Nico Iancoski GRR20210574

#ifndef _UTILS_
#define _UTILS_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macro para o codigo de erro da biblioteca
#define ERR { e_dados = 1; return NULL; }

// Codigo de erro da biblioteca
extern int e_dados;

// ********************************* Strings ******************************* //

// Retorna um ponteiro para uma string que comeca no final da tag
char *strstrP(char *str, char *tag);

// Concatena varias strings com um \n entre elas
char *myStrcat(char *str1, char *str2);

// Converte uma str para minuscula
char *myToLower(char *str);

// Inicia uma string com o primeiro valor em 0
char *iniciaStr();

// Retorna o valor de uma tag
char *tagValor(char *str, char *tag);

// Cria o nome do arquivo
char *nomeArq(char *nome_dir, char *nome_arq);

// ********************************* Vetores ******************************* //

// Retorna o tamanho da maior string no vetor
int calcMaxStr(char **v_str, int cnt);

// Retorna o maior valor de um vetor
int calcMax(int *v, int cnt);

// Retorna o maior valor em uma matriz
int calcMaxMatriz(int **m, int l, int c);

// Inicia um vetor com "tam" elementos em 0
int *iniciaVetor(int tam);

// Inicia uma matriz com l*c elementos em 0
int **iniciaMatriz(int l, int c);

// ********************************* Memoria ******************************* //

// Retorna NULL se cnt for 0, se não, retorna
// um ponteiro
void *myMalloc(size_t size, int cnt);

// Libera a memoria de um tanto variavel de parametros
void myFree(int n, ...);

#endif
