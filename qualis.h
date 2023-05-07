// Feito por Nico Iancoski GRR20210574

#ifndef _QUALIS_
#define _QUALIS_

#define C_TIPOS 2
#define C_NIVEL 10

// Niveis de publicacoes na Qualis
extern char *estratos[C_NIVEL];

// Representa um veiculo avaliado pela qualis
typedef struct qualis
{
  char *nome;
  char *nivel;

} t_qualis;

// ********************************* Funcoes ******************************* //

// Retorna um numero que representa um nivel Qualis
int nivelQualis(char *nivel);

// Cria um novo elemento to tipo qualis
t_qualis insereQualis(char *nome, char *nivel);

// Le todas as linhas de uma lista qualis, alocando em um vetor de
// struct qualis, que possui os campos nome e nivel
// Em caso de erro retorna NULL e seta em 1 o codigo de erro,
// em caso de sucesso retorna um ponteiro para a struct
t_qualis *leQualis(char *arq_nome, int *arq_cnt);

#endif
