#include <stdlib.h>
#include <stdio.h>
#include "horverao.h"

const char * const downames[] =
  { "dom", "seg", "ter", "qua", "qui", "sex", "sáb" };

int main(int argc, char *argv[]) {
  int dia = atoi(argv[1]), mes = atoi(argv[2]), ano = atoi(argv[3]);
  int diap, mesp, dds, res;
  if (argc < 4) {
	    fprintf(stderr, "Usage: horverao dia mes ano\n");
    exit(1);
  }
  calculaPascoa(&diap, &mesp, ano);
  printf("Páscoa para %d é no dia %02d/%02d\n", ano, diap, mesp);
  calculaDomingoCarnaval(&diap, &mesp, ano);
  printf("Domingo de carnaval de %d é no dia %02d/%02d\n", ano, diap, mesp);

  dds = diaDaSemana(dia, mes, ano);
  res = ehHorarioVerao(dia, mes, ano, dds);
  if (res == 1) {
    printf("O dia %02d/%02d/%d (%s) é durante o horário de verão\n",
           dia, mes, ano, downames[dds]);
  } else if (res == 0) {
    printf("O dia %02d/%02d/%d (%s) está fora do horário de verão\n",
           dia, mes, ano, downames[dds]);
  } else {
    printf("Erro! Ano %d < 2009!\n", ano);
  }

  return 0;
}
