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
  if (argc >= 5) {
    int hora = atoi(argv[4]);
    int minuto = (argc >= 6)? atoi(argv[5]) : 0;
    DiaDaSemana_t dds = diaDaSemana(dia, mes, ano);
    Horario_t isdst;
    printf("Data/hora UTC: %d-%02d-%02d %02d:%02d\n",
             ano, mes, dia, hora, minuto);
    utcParaHorarioLocal(&ano, &mes, &dia, &hora, &dds, &isdst,
                        Brasilia, PodeEntrarEmHorarioDeVerao);
    if (isdst > 0) {
      printf("Data/hora de verão de Brasília: %d-%02d-%02d %02d:%02d\n",
             ano, mes, dia, hora, minuto);
    } else {
      printf("Data/hora de Brasília: %d-%02d-%02d %02d:%02d\n",
             ano, mes, dia, hora, minuto);
    }
    return 0;
  }

  calculaPascoa(ano, &mesp, &diap);
  printf("Páscoa para %d é no dia %02d/%02d\n", ano, diap, mesp);
  calculaDomingoCarnaval(ano, &mesp, &diap);
  printf("Domingo de carnaval de %d é no dia %02d/%02d\n", ano, diap, mesp);

  dds = diaDaSemana(ano, mes, dia);
  res = horario(ano, mes, dia);
  if (res > 0) {
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
