// Feito por Nico Iancoski GRR20210574

#include <unistd.h>

#include "pub.h"
#include "utils.h"
#include "encerra.h"
#include "arquivos.h"
#include "estruturas.h"
#include "edit-distance.h"

// ********************************* Dados ********************************* //

// Funcao de comparacao para os dados
static int cmpDado(const void *s1, const void *s2)
{
  return strcmp( ((t_dado *)s1)->str, ((t_dado *)s2)->str);
}

// Redimensiona e ordena um vetor de dados
t_dado *redDado(t_dado *v, int cnt, int tam_atual)
{
  liberaDado(v, cnt, tam_atual);
  if ( cnt )
    if ( ! (v = realloc(v, sizeof(t_dado) * cnt)) ) { ERR; }
  qsort(v, cnt, sizeof(t_dado), cmpDado);

  return v;
} 
 
// Inicializa um vetor de cnt_dado elementos do tipo dado
t_dado *iniciaDado(int cnt_dado, int l, int c)
{
  t_dado *v_dado;
  if ( ! (v_dado = malloc(sizeof(t_dado) * cnt_dado)) ) { ERR; }
  for ( int i = 0; i < cnt_dado; i++ )
  {
    v_dado[i].cnt = 0;
    v_dado[i].str = iniciaStr();
    v_dado[i].autores = iniciaStr();
    v_dado[i].m_pub = iniciaMatriz(l, c);
  }

  return v_dado;
}

// Cria e retorna uma matriz com os totais de publicacao por estrato
int **valoresGrupo(t_grupo *g, int l, int c)
{
  int **val;
  
  if ( ! (val = iniciaMatriz(l, c+1)) ) { ERR; } 
  for ( int i = 0; i < l; i++ )
  {
    for ( int j = 0; j < c; j++)
      val[i][j] = somaNivel(g->m_pub[i][j], g->m_cnt[i][j]);
    
    val[i][c] = somaPub(g->m_pub[i], g->m_cnt[i]);
  }
  
  return val;
}

// ********************************* Grupo ********************************* //

// Inicializa a struct grupo, em caso de erro retorna NULL
t_grupo *iniciaGrupo(int cnt, char *nome)
{
  t_grupo *grupo;
  
  // Aloca a struct e os vetores
  if ( ! (grupo = malloc(sizeof(t_grupo))) ) { ERR; }
  if ( ! (grupo->v_ano = iniciaDado(cnt, C_TIPOS, C_NIVEL)) ) { ERR; }
  if ( ! (grupo->v_autor = iniciaDado(cnt, C_TIPOS, C_NIVEL)) ) { ERR; }

  // Aloca a matriz de publicacao e o contador
  for ( int t = 0; t < C_TIPOS; t++ )
    for ( int i = 0; i < C_NIVEL; i++ )
    {
      if ( ! (grupo->m_pub[t][i] = iniciaDado(cnt, 0, 0)) ) { ERR; }
      grupo->m_cnt[t][i] = 0;
    }

  grupo->nome = strdup(nome);
  grupo->cnt_ano = 0;
  grupo->cnt_autor = 0;
  
  return grupo;
}

// Cria um grupo com as publicacoes e retorna um ponteiro para ele, 
// em caso de falha retorna NULL
t_grupo *criaGrupo(t_lattes *lattes, t_f **vf_pub, char *nome)
{
  t_grupo *g;
  
  // Aloca o maior tamanho que pode ser necessario para depois realocar
  int tam_fila = tamFila(vf_pub[conf]) + tamFila(vf_pub[per]);
  if ( ! (g = iniciaGrupo(tam_fila, nome)) ) { ERR; }

  // Adiciona os dados dos periodicos
  g = dadosGrupo(vf_pub[per], lattes->vq_per, lattes->cnt_per, g, per);
  
  // Adiciona os dados das conferencias
  g = dadosGrupo(vf_pub[conf], lattes->vq_conf, lattes->cnt_conf, g, conf);

  // Redimensiona os vetores para ocupar somente o necessario
  if ( ! (g->v_ano = redDado(g->v_ano, g->cnt_ano, tam_fila)) ) { ERR; }
  if ( ! (g->v_autor = redDado(g->v_autor, g->cnt_autor, tam_fila)) ) { ERR; }
  for ( int t = 0; t < C_TIPOS; t++ )
    for ( int i = 0; i < C_NIVEL; i++ )
    {  
      g->m_pub[t][i] = redDado(g->m_pub[t][i], g->m_cnt[t][i], tam_fila);
      if ( e_dados ) { ERR; }
    } 
  
  return g;
}

// Calcula o valor maximo de publicacoes de um grupo
int calculaMaxGeral(t_grupo **g, int cnt_g, int l, int c)
{
  int max_tmp, **val, max_geral = 0;
  for ( int i = 0; i < cnt_g; i++ )
  {
    val = valoresGrupo(g[i], l, c);
    if ( !val ) { return 0; } 
    max_tmp = calcMaxMatriz(val, C_TIPOS, C_NIVEL+1);
    if ( max_geral < max_tmp ) { max_geral = max_tmp; }
    liberaMatriz(val, l);
  }

  return max_geral;
}

// ********************************* Lattes ******************************** //

// Inicializa uma estrutura para armazenar os dados lidos em cada diretorio
t_lattes *iniciaLattes(t_qualis *vq_per, t_qualis *vq_conf, int dir_cnt, 
                       int cnt_per, int cnt_conf )
{
  t_lattes *lattes;
  if ( ! (lattes = malloc(sizeof(t_lattes))) ) { ERR; }
  if ( ! (lattes->grupo = malloc(sizeof(t_grupo *) * dir_cnt)) ) { ERR; }
 
  lattes->cnt_g = dir_cnt;
  lattes->cnt_per = cnt_per;
  lattes->cnt_conf = cnt_conf;
  lattes->vq_per = vq_per;
  lattes->vq_conf = vq_conf;
  
  return lattes;
}

// Insere todos os valores lidos na estrutura e libera parte da memoria
t_lattes *insereLattes(t_lattes *lattes, t_f ***mf_pub, char **nome_dir)
{
  int cnt_g = lattes->cnt_g;

  // Adiciona os dados de cada grupo
  for ( int i = 0; i < cnt_g; i++ )
  {
    lattes->grupo[i] = criaGrupo(lattes, mf_pub[i], nome_dir[i]);
    for ( int j = 0; j < C_TIPOS; j++ ) { free(mf_pub[i][j]); }
  }

  // Libera a memoria ocupada pelas filas
  for ( int i = 0; i < cnt_g; i++ ) { free(mf_pub[i]); }
  
  myFree(2, mf_pub, nome_dir);
  return lattes;
}

// Le as listas da qualis e os curriculos de um grupo Em caso de 
// erro retorna NULL e seta em 1 o codigo de erro da biblioteca,
// em caso de sucesso retorna um ponteiro para a struct
t_lattes *leLattes(int argc, char **argv)
{
  int op, cnt_per, cnt_conf;
  char **nome_dir;
  t_qualis *vq_per = NULL, *vq_conf = NULL;
  t_f ***mf_pub = NULL;

  // Aloca um vetor para os nomes dos diretorios
  int dir_cnt = argc-6;
  if ( ! (nome_dir = malloc(sizeof(char *) * dir_cnt)) ) { ERR; }

  while ( (op = getopt(argc, argv, "p:c:d:")) != -1 )
  {
    switch ( op )
    {
      case 'p':
        vq_per = leQualis(optarg, &cnt_per);
        break;

      case 'c':
        vq_conf = leQualis(optarg, &cnt_conf);
        break;

      case 'd':
        mf_pub = leDir(argv, --optind, dir_cnt, nome_dir);
        optind += dir_cnt;
        break;

      case '?':
      case ':':
      default:
        ERR;
        break;
    }
  }

  if ( e_dados ) { ERR; }
  t_lattes *lattes = iniciaLattes(vq_per, vq_conf, dir_cnt, cnt_per, cnt_conf);
  
  return insereLattes(lattes, mf_pub, nome_dir);
}
