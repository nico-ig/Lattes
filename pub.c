// Feito por Nico Iancoski GRR20210574

#include "pub.h"
#include "utils.h"
#include "qualis.h"
#include "encerra.h"
#include "estruturas.h"
#include "edit-distance.h"

// Vetor com os nomes dos tipos de publicacoes
char *v_labels[C_TIPOS] = { "Periodicos", "Conferencias" };

// Representa uma publicacao, independente do tipo
typedef struct publicacao
{
  char *titulo, *ano, *veiculo, *autor, *autores;

} t_pub;

// ******************************** Autores ******************************** //

// Le todos os autores de uma publicacao
char *leAutores(char *str, char *fim)
{
  char *co_autor;
  char *autores = iniciaStr();

  do
  {
    // Acerta a posicao do ponteiro
    if ( ! (str = strstr(str, "AUTORES")) ) { break; }
    
    co_autor = tagValor(str, "NOME-COMPLETO-DO-AUTOR");
    
    // Adiciona o co_autor caso seja a primeira vez que aparece
    if ( editDistanceStr(autores, co_autor) )
      autores = myStrcat(autores, co_autor);   
   
    free(co_autor);
    str = strstrP(str, "/>");
  } while ( strncmp(str, fim, strlen(fim)) );

  return autores;
}

// Adiciona os co_autores de uma publicacao ao autor
char *adicionaAutores(char *autores, char *co_autores)
{
  char *co_autor = strtok(co_autores, "\n");
  while ( co_autor != NULL )
  {
    if ( editDistanceStr(autores, co_autor) )
      autores = myStrcat(autores, co_autor);

    co_autor = strtok(NULL, "\n");
  }  

  return autores;
}

// ****************************** Publicacoes ****************************** //

// Soma todos os elementos em um nivel
int somaNivel (t_dado *v, int cnt)
{
  int total = 0;
  for ( int i = 0; i < cnt; i++ )
    total += v[i].cnt;

   return total;
}

// Soma todos os elementos de um tipo de publicacao
int somaPub(t_dado **v_pub, int *v_cnt)
{
  int total = 0;
  for ( int i = 0; i < C_NIVEL; i++ )
    total += somaNivel(v_pub[i], v_cnt[i]);
  
  return total;
}

// Funcao auxiliar para ler publicacoes, insere cada publicacao em uma fila
// e retorna um ponteiro para ela, em caso de erro retorna NULL
t_f *lePub_(char *str, char *ini, char* tit, char *ano, 
            char *vei, char *fim, char* autor, t_f *f)
{
  char *ptr;
  
  t_pub *pub;
  while ( (ptr = strstr(str, ini)) )
  {
    if ( ! (pub = malloc(sizeof(t_pub))) ) { ERR; }
   
    pub->autor = strdup(autor);
    pub->titulo = tagValor(str, tit);
    pub->ano = tagValor(str, ano);
    pub->veiculo = tagValor(str, vei);
    pub->autores = leAutores(str, fim);
    str = strstrP(str, fim);

    if ( ! insereFila(f, pub) ) { ERR; }
  }
  return f;
}

// Le todas as publicacacoes, tanto de periodicos como de conferencias, e 
// retorna um ponteiro para uma fila contendo as publicacoes do um tipo
t_f *lePub(char *str, e_tipo t, char *autor, t_f *f)
{
  char *ini = NULL, *titulo = NULL, *ano = NULL, *veiculo = NULL, *fim = NULL;

  switch (t)
  {
    case per:
      ini = "ARTIGO-PUBLICADO";
      titulo =  "TITULO-DO-ARTIGO";
      ano = "ANO-DO-ARTIGO";
      veiculo = "TITULO-DO-PERIODICO-OU-REVISTA";
      fim = "</ARTIGO-PUBLICADO>"; 
      break;

    case conf:
      ini = "TRABALHO-EM-EVENTOS";
      titulo = "TITULO-DO-TRABALHO";
      ano = "ANO-DO-TRABALHO";
      veiculo = "NOME-DO-EVENTO"; 
      fim = "</TRABALHO-EM-EVENTOS>";
      break;

    default:
      break;
  }

  return lePub_(str, ini, titulo, ano, veiculo, fim, autor, f);
}

// Retorna o nivel de uma publicacao, em caso de erro retorna -1
int nivelPub(t_pub *pub, t_qualis *v_qualis, int cnt_q)
{
    int i;
    char *n_qualis, *n_veiculo;
    
    n_veiculo = myToLower(pub->veiculo);
    
    for ( i = 0; i < cnt_q; i++ )
    {
      n_qualis = myToLower(v_qualis[i].nome);
    
      // Se as str forem equivalentes retorna o nivel do dado ja inserido
      if ( !editDistanceBool(n_qualis, n_veiculo) )
      {
        myFree(3, pub->veiculo, n_qualis, n_veiculo);
        pub->veiculo = strdup(v_qualis[i].nome);
        return nivelQualis(v_qualis[i].nivel);
      }
      
      free(n_qualis);
    }

    free(n_veiculo);

    // Se nao encontrou, entao o nivel eh NC
    if ( i == cnt_q )
      return nivelQualis("NC");

    return -1;
}

void destroiPub(t_pub *pub)
{
  myFree(6, pub->autores, pub->autor, pub->ano, pub->veiculo, pub->titulo, pub);
}

// ********************************* Dados ********************************* //

// Atualiza o dado de acordo com as informacoes a serem adicionadas
t_dado *atuDado(t_dado *v, int *tam, int nivel, char *str, char *autores, 
                      e_tipo t, int (cmpFunc)(const char *, const char *))
{ 
  int i, achou = 0;
  char *str1, *str2;
  
  // Ve se ja foi adicionado um dado com o mesmo nome
  for ( i = 0 ; i < *tam; i++ )
  {
    str1 = myToLower(v[i].str);
    str2 = myToLower(str);
    if ( !cmpFunc(str1, str2) ) { achou = 1; myFree(2, str1, str2); break; }
    myFree(2, str1, str2);
  }

  if ( !achou )
  {
    myFree(2, v[i].autores, v[i].str);
    v[i].str = strdup(str);
    v[i].autores = strdup(autores);
    (*tam)++;
  }

  else
    v[i].autores = adicionaAutores(v[i].autores, autores); 
  
  // Caso a posicao da matriz tenha sido alocada, atualiza o valor
  if ( nivel != -1 )
    v[i].m_pub[t][nivel]++;
  
  v[i].cnt++;
  return v;
}

// ********************************* Grupo ********************************* //

// Atualiza os valores do grupo com os dados da ultima publicacao lida    
t_grupo *atuGrupo(t_grupo *g, int n, t_pub *pub, e_tipo t, char **v_str)
{
  // Atribuicoes para facilitar a leitura
  char *autores = pub->autores;
  char *titulo = pub->titulo;
  char *autor = pub->autor;
  char *vei = pub->veiculo;
  char *ano = pub->ano;

  int *m_cnt = &(g->m_cnt[t][n]);

  t_dado *v_autor = g->v_autor;
  t_dado *v_ano = g->v_ano;
  t_dado *m_pub = g->m_pub[t][n];

  v_autor = atuDado(v_autor, &g->cnt_autor, n, autor, autores, t, strcmp);
 
  //Verifica se ja foi adicionado uma publicacao com esse titulo ao grupo
  if ( editDistanceStr(v_str[t], titulo) )
  {
    v_str[t] = myStrcat(v_str[t], titulo); 
    v_ano = atuDado(v_ano, &g->cnt_ano, n, ano, autores, t, strcmp);
    m_pub = atuDado(m_pub, m_cnt, -1, vei, autores, t, editDistanceBool);
  }
    
  return g;
}

// Utiliza os valores de uma lista qualis para classificar as publicacoes em 
// uma fila de publicacao e preencher os dadoes referentes a elas em um grupo. 
// Retorna um ponteiro para o grupo e, em caso de falha, retorna NULL
t_grupo *dadosGrupo(t_f *f_pub, t_qualis *v_q, int cnt_q, t_grupo *g, e_tipo t)
{
  int nivel;
  char **v_str;

  if ( ! (v_str = malloc(sizeof(char *) * C_TIPOS)) ) { ERR; }
  for ( int i = 0; i < C_TIPOS; i++ ) { v_str[i] = iniciaStr(); }

  // Esvazia a fila vendo qual o nivel de cada publicacao
  t_pub *pub;
  while ( retiraFila(f_pub, &pub) )
  {
    nivel = nivelPub(pub, v_q, cnt_q); 
    g = atuGrupo(g, nivel, pub, t, v_str);
    destroiPub(pub); 
  }

  liberaVStr(v_str, C_TIPOS);
  return g;
}


