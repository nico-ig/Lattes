// Feito por Nico Iancoski GRR20210574

#include <dirent.h>

#include "pub.h"
#include "utils.h"
#include "qualis.h"
#include "arquivos.h"
#include "estruturas.h"

#define TAB 2

// ********************************* Arquivos ****************************** //

// Le todas as publicacoes de um arquivo, retorna um vetor com duas filas,
// uma de publicacoes em periodicos e outra de publicacoes em conferencias. 
// Em caso de erro retorna NULL
t_f **leArq(char *n_dir, char *n_arq, t_f **vf_pub)
{
  FILE *arq; 
  char *arq_str, *str, *nome_arq;
  
  nome_arq = nomeArq(n_dir, n_arq);
  if ( ! (arq = fopen(nome_arq, "r")) ) { ERR; }
  
  // Le todo o arquivo em uma unica string
  fscanf(arq, "%m[^\n]", &arq_str);

  char *tag_autor = "NOME-COMPLETO";
  char *autor = tagValor(arq_str, tag_autor);

  // Le todos os artigos publicados
  str = strstrP(arq_str, "ARTIGOS-PUBLICADOS");
  printf("\n%*sLendo artigos", TAB*2, " ");
  vf_pub[per] = lePub(str, per, autor, vf_pub[per]);

  // Le todos os trabalhos publicados
  str = strstrP(arq_str, "TRABALHOS-EM-EVENTOS");
  printf("\n%*sLendo conferencias", TAB*2, " ");
  vf_pub[conf] = lePub(str, conf, autor, vf_pub[conf]);

  myFree(3, nome_arq, arq_str, autor);
  fclose(arq); 
  
  return vf_pub;
}

// ******************************* Diretorios ****************************** //

// Funcao auxiliar que le apenas um diretorio
// Retorna vetor com C_TIPOS filas ou NULL em caso de falha
t_f **leDir_(char *nome_dir)
{
  DIR *dirstream; struct dirent *direntry;
  if ( ! (dirstream = opendir(nome_dir)) ) { ERR; }
  
  // Cria um vetor com C_TIPOS filas:
  t_f **vf_pub;
  if ( ! (vf_pub = malloc(sizeof(t_f *) * C_TIPOS)) ) { ERR; }
  for ( int i = 0; i < C_TIPOS; i++ ) { vf_pub[i] = criaFila(); }

  printf("\nLendo diretorio %s", nome_dir);
  
  // Le os arquivos do diretorio e insere no vetor de filas
  while ( (direntry = readdir(dirstream)) )
  {
    if ( direntry->d_type == DT_REG )
    {
      printf("\n%*sLendo arquivo %s", TAB, " ", direntry->d_name);
      vf_pub = leArq(nome_dir, direntry->d_name, vf_pub);
      if ( ! vf_pub ) { ERR; }
    }
  }
 
  printf("\n");
  closedir(dirstream);
  return vf_pub;
}

// Le todos os arquivos em todos os diretorios e retorna um ponteiro para uma
// matriz de vetores, cada um com com duas filas: uma das publicacoes em 
// periodicos e outra das publicacoes em conferencias. 
// Em caso de erro retorna NULL
t_f ***leDir(char **argv, int optind, int dir_cnt, char **nome_dir)
{
  // Aloca espaco para as filas de acordo com a qtd de diretorios
  t_f ***mf_pub;
  if ( ! (mf_pub = malloc(sizeof(t_f **) * dir_cnt)) ) { ERR; }
 
  // Le todos os diretorios
  for ( int i = 0; i < dir_cnt; i++ )
  {
    nome_dir[i] = argv[optind++];
    mf_pub[i] = leDir_(nome_dir[i]);
  }

  return mf_pub;
}
