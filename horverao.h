#ifndef HORVERAO_H
#define HORVERAO_H

/**
   Determina se o dia está dentro ou fora do horário de verão.
   O critério usado é o do Decreto N° 6.558 de 08/09/2008.
   @param dia Dia do mês
   @param mes Mês entre 1 e 12
   @param ano Ano a partir de 2009
   @param dds Dia da semana: 0 (dom), 1 (seg), ..., 6 (sáb)
   @return 1 se o dia está dentro do horário de verão,
           0 se está fora, -1 se o ano é menor do que 2009.
 */
int ehHorarioVerao(int dia, int mes, int ano, int dds);

void calculaPascoa(int *dia, int *mes, int ano);
void calculaDomingoCarnaval(int *dia, int *mes, int ano);
int diaDaSemana(int dia, int mes, int ano);

#endif
