// Feito por Nico Iancoski GRR20210574

#include "utils.h"
#include "encerra.h"
#include "estruturas.h"
#include "edit-distance.h"

// ******************************** Vetores ******************************** //

// Libera toda a memoria utilizada por uma matriz
void liberaMatriz(int **m, int cnt)
{
  for ( int i = 0; i < cnt; i++ )
      free(m[i]);
  free(m);
}

// Libera toda a memoria utilizada por um vetor de strings
void liberaVStr(char **v_str, int cnt)
{
  for ( int i = 0; i < cnt; i++ ) { free(v_str[i]); }
  free(v_str);
}

// Libera toda a memoria utilizada por um vetor qualis
void liberaQualis(t_qualis *v, int cnt)
{
  for ( int i = 0; i < cnt; i++ )
    myFree(2, v[i].nome, v[i].nivel);
  
  free(v);
}

// Libera toda a memoria utilizada pelo vetor de anos
void liberaDado(t_dado *v_dado, int ini, int fim)
{
  for ( int i = ini; i < fim; i++ )
  {
    if ( v_dado[i].m_pub )
      liberaMatriz(v_dado[i].m_pub, C_TIPOS);
    myFree(2, v_dado[i].autores, v_dado[i].str);
  }

  if ( !ini && fim )
    free(v_dado);
}


// ****************************** Estruturas ******************************* //

// Libera toda a memoria utilizada pelo grupo
void liberaGrupo(t_grupo *g)
{
  liberaDado(g->v_ano, 0, g->cnt_ano);
  liberaDado(g->v_autor, 0, g->cnt_autor);
  
  for ( int i = 0; i < C_TIPOS; i++ )
    for ( int j = 0; j < C_NIVEL; j++ )
      liberaDado(g->m_pub[i][j], 0, g->m_cnt[i][j]);

  myFree(2, g->nome, g);
}

// Libera toda a memoria em uso ao final da execucao
void encerra(t_lattes *lattes)
{
  // Libera a memoria utilizada pela funcao editDistance(), 
  // utilizada para fazer a classificacao das publicacoes
  liberaEditDistance();

  liberaQualis(lattes->vq_per, lattes->cnt_per);
  liberaQualis(lattes->vq_conf, lattes->cnt_conf);
  
  for ( int i = 0; i < lattes->cnt_g; i++ )
    liberaGrupo(lattes->grupo[i]);
  
  myFree(2, lattes->grupo, lattes);
}

