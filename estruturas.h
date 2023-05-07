// Feito por Nico Iancoski GRR20210574

#ifndef _ESTRUTURAS_
#define _ESTRUTURAS_

#include "qualis.h"

typedef struct fila t_f;

// ********************************* Structs ******************************* //

// Representa os dados com as informacoes de interesse
typedef struct dado
{
  char *str, *autores;
  int cnt, **m_pub;

} t_dado;

// Representa um grupo
typedef struct grupo
{
  int cnt_ano, cnt_autor, m_cnt[C_TIPOS][C_NIVEL];
  t_dado *v_ano, *v_autor, *m_pub[C_TIPOS][C_NIVEL];
  char *nome;

} t_grupo;

// Representa todos os dados lidos: 
// grupo, lista de conferencia e de periodicos
typedef struct lattes
{
  t_qualis *vq_per, *vq_conf;
  int cnt_per, cnt_conf, cnt_g;
  t_grupo **grupo;

}t_lattes;

// ********************************* Dados ********************************* //

// Cria e retorna uma matriz com os totais de publicacao por estrato
int **valoresGrupo(t_grupo *g, int l, int c);

// ********************************* Grupo ********************************* //

// Calcula o valor maximo de publicacoes de um grupo
int calculaMaxGeral(t_grupo **g, int cnt_g, int l, int c);

// ********************************* Lattes ******************************** //

// Inicializa uma estrutura para armazenar os dados lidos em cada diretorio
t_lattes *iniciaLattes(t_qualis *vq_per, t_qualis *vq_conf, int dir_cnt, 
                       int cnt_per, int cnt_conf );

// Insere todos os valores lidos na estrutura e libera parte da memoria
t_lattes *insereLattes(t_lattes *lattes, t_f ***mf_pub, char **nome_dir);

// Le as listas da qualis e os curriculos de um grupo. Em caso de 
// erro retorna NULL e seta em 1 o codigo de erro da biblioteca,
// em caso de sucesso retorna um ponteiro para a struct
t_lattes *leLattes(int argc, char **argv);

#endif
