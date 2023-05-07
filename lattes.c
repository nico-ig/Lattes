// Feito por Nico Iancoski GRR20210574

#include "utils.h"
#include "encerra.h"
#include "interface.h"
#include "estruturas.h"

int main(int argc, char **argv)
{
  t_lattes *lattes = leLattes(argc, argv);

  if ( e_dados ) { return 1; }

  //menu(lattes);
  relatorio(lattes);

  encerra(lattes);

  return 0;
}
