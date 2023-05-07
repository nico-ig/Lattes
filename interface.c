// Feito por Nico Iancoski GRR20210574

#include "pub.h"
#include "utils.h"
#include "qualis.h"
#include "grafico.h"
#include "encerra.h"
#include "interface.h"

#define TAMGi 80
#define TAMG 51

// Macro que limpa o terminal
#define CLEAR { printf("\e[1;1H\e[2J"); }

// Enum para facilitar a leitura do menu
typedef enum opcoes {t_relatorio, t_menu, voltar_co=1, comp_grafico=1, 
                     sum_per=1, sum_conf, ano, autor, nivelC, nivelNC, 
                     co_autores, grafico, voltar} e_op;

char *op_grupo[] = { "Encerrar", "Producao por ordem de periodicos",
                     "Producao por ordem de conferencias",
                     "Producao por ordem de ano",        
                     "Producao por ordem de autoria",        
                     "Periodicos e eventos no nivel C",
                     "Periodicos e eventos no nivel NC",
                     "Co-Autores", "Grafico", "Voltar" };

int op_grupo_tam = 10;

// ********************************* Linhas ******************************** //

// Imprime uma linha de tamanho "tam"
void imprimeLinha(int tam)
{
  for ( int i = 0; i < tam; i++)
    printf("-");
  printf("\n");
}

// Imprime uma linha separando em cnt_t campos
void imprimeLinhaMais(int cnt_l, int cnt_t )
{
  for ( int i = 0; i < cnt_t; i++ )
  {
    printf("+");
    for ( int j = 0; j < cnt_l; j++ ) { printf("-"); }
  }
  printf("+\n");
}

// ******************************** Cabecalhos ***************************** //

// Imprime o cabecalho para a tabela de dados
void imprimeCabecalhoDado()
{
  int max_label = calcMaxStr(v_labels, C_TIPOS);
  imprimeLinhaMais(max_label, C_TIPOS);
  for ( int i = C_TIPOS-1; i >= 0; i-- )
    printf("|%-*s", max_label, v_labels[i]);
  printf("|\n");
  imprimeLinhaMais(max_label, C_TIPOS);
}

// Imprime o cabecalho da opcao escolhida
void imprimeCabecalho(char *str)
{
  printf("\n");
  imprimeLinhaMais(TAMG, 1);
  int tam_str = (int)TAMG-strlen(str);
  int offset = tam_str/2;
  printf("|%*s%s%*s|\n", offset, " ", str, offset+(tam_str%2), " ");
  imprimeLinhaMais(TAMG, 1);
  printf("\n");
}


// ********************************* Dados ********************************* //

// Imprime um vetor sumarizado
void imprimeVPub(t_dado **v, int *cnt)
{
  for ( int i = 0; i < C_NIVEL-2; i++ )
  {
  printf("Estrato: %s\n", estratos[i]);
    for ( int j = 0; j < cnt[i]; j++ )
      printf("%s: %d\n", v[i][j].str, v[i][j].cnt);
    printf("\n");
  }

  // Imprime o nivel C e NC junto
  printf("Estrato: %s\n", estratos[C_NIVEL-2]);
  for ( int i = 2; i > 0; i-- )
  {
    for ( int j = 0; j < cnt[C_NIVEL-i]; j++ )
      printf("%s: %d\n", v[C_NIVEL-i][j].str, v[C_NIVEL-i][j].cnt);
  }
  
  printf("\n");
}

void imprimeNivel(t_dado *m_pub[C_TIPOS][C_NIVEL], 
                  int m_cnt[C_TIPOS][C_NIVEL], int n)
{
  for ( int t = 0; t < C_TIPOS; t++ )
  {
    printf("%s\n", v_labels[t]);
    imprimeLinha(TAMGi);

    for ( int i = 0; i < m_cnt[t][n]; i++ )
      printf("%s\n", m_pub[t][n][i].str);

    printf("\n");
  }

}

// Imprime a sumarizacao por elemento do vetor de dado
void imprimeDado(t_dado *v_dado, int tam, char *tipo)
{
  for ( int i = tam-1; i >= 0; i-- )
  {
    printf("%s %s\n", tipo, v_dado[i].str);
    imprimeCabecalhoDado();
    
    for ( int n = 0; n < C_NIVEL-3; n++ )
    {
      for ( int t = C_TIPOS-1; t >= 0; t-- )
        printf("| %-2s: %-7d", estratos[n], v_dado[i].m_pub[t][n]);
      printf("|\n");
    }

    // Imprime a soma do nivel C e NC junto
    for ( int t = C_TIPOS-1, total = 0; t >= 0; t--, total = 0 )
    { 
      total = v_dado[i].m_pub[t][C_NIVEL-2] + v_dado[i].m_pub[t][C_NIVEL-1];
      printf("| %-2s: %-7d", estratos[C_NIVEL-2], total);
    }

    printf("|\n");
    imprimeLinhaMais(calcMaxStr(v_labels, C_TIPOS), C_TIPOS);
    printf("\n");
  }
}

// Imprime a lista de co-autoria de um autor
void imprimeCoAutores(t_dado autor, int tipo)
{
  char *nome = autor.str;
  char *autores = autor.autores;

  if ( tipo == t_menu )
    imprimeCabecalho("Co-Autoria");
  
  printf("Autor: %s\n", nome);
  imprimeLinha(TAMG);
 
  char *ini = strstr(autores, nome);
  char *fim = strstrP(autores, nome)+1;
  char *aux1 = strndup(autores, ini-autores);
  printf("%s", aux1);
  printf("%s\n\n", fim);
  
  free(aux1);
}

// ********************************* Graficos ****************************** //


// Imprime um grafico com a qtd de pub em periodicos e eventos
void imprimeGrafico(t_grupo *g, int l, int c, char *titulo, int max_y)
{
  char **l_valores;

  int **val = valoresGrupo(g, C_TIPOS, C_NIVEL);
  if ( !val ) { return; }

  // Adiciona o label de total
  if ( ! (l_valores = malloc(sizeof(char *) * (c+1))) ) { return; }
  memcpy(l_valores, estratos, sizeof(char *) * c);
  l_valores[c] = "Total";

  plotGrafico(val, l, c+1, v_labels, l_valores, titulo, max_y);
  
  free(l_valores);
  liberaMatriz(val, l);
}

// Imprime os graficos de todos os grupos
void imprimeGraficos(t_grupo **g, int cnt_g)
{
  int max_geral = calculaMaxGeral(g, cnt_g, C_TIPOS, C_NIVEL);
  imprimeCabecalho("Graficos");

  for ( int i = 0; i < cnt_g; i++ )
    imprimeGrafico(g[i], C_TIPOS, C_NIVEL, g[i]->nome, max_geral);
}

// ********************************* Opcoes ******************************** //

// Imprime todos os autores do grupo
void imprimeMenuCoAutores(t_dado *v_autor, int cnt)
{
  printf("Selecione o autor:\n");
  imprimeLinha(TAMG);
  printf("0. Encerrar\n");
  printf("1. Voltar\n");
  for ( int i = 0; i < cnt; i++ )
    printf("%d. %s\n", i+2, v_autor[i].str);
  imprimeLinha(TAMG);
  printf("\n");
}

// Imprime as opcoes do menu
void imprimeMenuGrupo()
{
  printf("Selecione a opcao desejada:\n");
  imprimeLinha(TAMG);
  for ( int i = 0; i < op_grupo_tam; i++ )
    printf("%d. %s.\n", i, op_grupo[i]);
  imprimeLinha(TAMG);
}

// Imprime as opcoes gerais
void imprimeMenuGeral(t_grupo **g, int cnt_g)
{
  printf("Selecione a opcao desejada:\n");
  imprimeLinha(TAMG);
  printf("0. Encerrar\n");
  printf("1. Comparar grupos\n");

  for ( int i = 2; i < cnt_g+2; i++ )
    printf("%d. Informacoes do grupo %s\n", i, g[i-2]->nome);
  imprimeLinha(TAMG);
}

// Imprime todos os autores do grupo
int opcoesCoAutores(t_dado *v_autor, int cnt)
{
  int op;
  imprimeMenuCoAutores(v_autor, cnt); 
  printf("Opcao: ");
  scanf("%d", &op);
  return op;
}

// Gerencia a chamada de opcoes do grupo
int opcoesGrupo()
{
  int op;
  imprimeMenuGrupo();
  printf("Opcao: ");
  scanf("%d", &op);
  return op;
}

// Gerencia a chamada de opcoes gerais
int opcoesGeral(t_grupo **g, int cnt_g)
{
  int op;
  imprimeMenuGeral(g, cnt_g);
  printf("Opcao: ");
  scanf("%d", &op);
  return op;
}

// *********************************** Menus ******************************* //

// Imprime e gerencia o menu de co-autoria
int menuCoAutores(t_dado *v_autor, int cnt)
{
  int op; 
  CLEAR;
  
  while ( (op = opcoesCoAutores(v_autor, cnt)) != 0 )
  {
    CLEAR;
    switch (op)
    {
      case voltar_co:
        break;

      default:
        imprimeCoAutores(v_autor[op-2], t_menu);
        break;
    }
  
    if ( op == voltar_co) { break; }
  }
  
  CLEAR;
  return (!op);
}

// Imprime o menu para um grupo e gerencia a chamada das opcoes
int menu_grupo(t_grupo *g)
{
  int op, exit = 0, max_y, **val;

  CLEAR;
  while ( (op = opcoesGrupo()) != 0 )
  {
    CLEAR;
    imprimeCabecalho(op_grupo[op]);

    switch (op)
    {
      case sum_per:
      case sum_conf:
        imprimeVPub(g->m_pub[op-1], g->m_cnt[op-1]);
        break;

      case ano:
        imprimeDado(g->v_ano, g->cnt_ano, "Ano:");
        break;

      case autor:
        imprimeDado(g->v_autor, g->cnt_autor, "Autor:");
        break;

      case nivelC:
        imprimeNivel(g->m_pub, g->m_cnt, C_NIVEL-2);
        break;

      case nivelNC:
        imprimeNivel(g->m_pub, g->m_cnt, C_NIVEL-1);
        break;

      case co_autores:
        exit = menuCoAutores(g->v_autor, g->cnt_autor);
        CLEAR;
        break;

      case grafico:
        val = valoresGrupo(g, C_TIPOS, C_NIVEL);
        max_y = calcMaxMatriz(val, C_TIPOS, C_NIVEL+1);
        imprimeGrafico(g, C_TIPOS, C_NIVEL, g->nome, max_y);
        liberaMatriz(val, C_TIPOS);
        break;

      case voltar:
        break;

      default:
        printf("Opcao invalida\n");
        break;
    }
  
    if ( (op == voltar) || exit ) break;

  }
  CLEAR;
  return (exit || !op);
}

// Imprime o menu geral e gerencia a chamada das opcoes
void menu(t_lattes *lattes)
{
  int op, exit; 
  int cnt_g = lattes->cnt_g;
  t_grupo **g = lattes->grupo;
  CLEAR;

  while ( (op = opcoesGeral(g, cnt_g)) != 0 )
  {
    CLEAR;
    switch (op)
    {
      case comp_grafico:
        imprimeGraficos(g, cnt_g);
        break;

      default:
        exit = menu_grupo(lattes->grupo[op-2]);
        CLEAR;
        break;
    }

    if ( exit ) break;
  }
  CLEAR;
}

// ******************************* Relatorio ******************************* //

// Imprime o relatorio de co-autoria
void relatorioCoAutores(t_dado *v_autor, int cnt)
{
  for ( int op = 0; op < cnt; op++ )
    imprimeCoAutores(v_autor[op], t_relatorio);
}

// Imprime o relatorio para um grupo
void relatorio_grupo(t_grupo *g)
{
  imprimeCabecalho(g->nome);
  for ( int op = 1; op < op_grupo_tam-2; op++ )
  {
    imprimeCabecalho(op_grupo[op]);

    switch (op)
    {
      case sum_per:
      case sum_conf:
        imprimeVPub(g->m_pub[op-1], g->m_cnt[op-1]);
        break;

      case ano:
        imprimeDado(g->v_ano, g->cnt_ano, "Ano:");
        break;

      case autor:
        imprimeDado(g->v_autor, g->cnt_autor, "Autor:");
        break;

      case nivelC:
        imprimeNivel(g->m_pub, g->m_cnt, C_NIVEL-2);
        break;

      case nivelNC:
        imprimeNivel(g->m_pub, g->m_cnt, C_NIVEL-1);
        break;

      case co_autores:
        relatorioCoAutores(g->v_autor, g->cnt_autor);
        break;

      default:
        printf("Opcao invalida\n");
        break;
    }
  }
}

// Imprime o relatorio com as informacoes lidas
void relatorio(t_lattes *lattes)
{
  int cnt_g = lattes->cnt_g;
  t_grupo **g = lattes->grupo;

  for( int op = 1; op <= cnt_g+1; op++ )
  {
    switch (op)
    {
      case comp_grafico:
        imprimeGraficos(g, cnt_g);
        break;

      default:
        relatorio_grupo(lattes->grupo[op-2]);
        break;
    }
  }
}
