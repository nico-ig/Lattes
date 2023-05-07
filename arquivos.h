// Feito por Nico Iancoski GRR20210574

#ifndef _ARQUIVOS_
#define _ARQUIVOS_

// Le todos os arquivos em todos os diretorios e retorna um ponteiro para uma
// matriz de vetores, cada um com com duas filas: uma das publicacoes em 
// periodicos e outra das publicacoes em conferencias. 
// Em caso de erro retorna NULL
t_f ***leDir(char **argv, int optind, int dir_cnt, char **nome_dir);

#endif
