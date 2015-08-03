#ifndef HORVERAO_H
#define HORVERAO_H

typedef enum { Indefinido = -1,
               Normal = 0,
               HorarioDeVerao = 1,
               UltimoDiaHorarioDeVerao = 2 } Horario_t;

typedef enum { Domingo = 0,     SegundaFeira = 1,
               TercaFeira = 2,  QuartaFeira = 3,
               QuintaFeira = 4, SextaFeira = 5,
               Sabado = 6 } DiaDaSemana_t;

typedef enum { FernandoDeNoronha = -2,
               Brasilia = -3,
               Amazonia = -4,
               Acre = -5 } Fuso_t;

typedef enum { NuncaEntraEmHorarioDeVerao = 0,
               PodeEntrarEmHorarioDeVerao = 1 } Regra_t;

DiaDaSemana_t diaDaSemana(int ano, int mes, int dia);

void utcParaHorarioLocal(int *ano, int *mes, int *dia, int *hora,
                         DiaDaSemana_t *dds, Horario_t *isdst,
                         Fuso_t fuso, Regra_t regra);

void inicioHorarioVerao(int ano, int *mes, int *dia);
void fimHorarioVerao(int ano, int *mes, int *dia);

/**
   Determina se o dia está dentro ou fora do horário de verão.
   O critério usado é o do Decreto N° 6.558 de 08/09/2008.
   @param dia Dia do mês
   @param mes Mês entre 1 e 12
   @param ano Ano a partir de 2009
   @return 1 ou 2 se o dia está dentro do horário de verão (2 se é
           o último dia do horário de verão),
           0 se está fora, -1 se o ano é menor do que 2009.
 */
Horario_t horario(int ano, int mes, int dia);

void calculaPascoa(int ano, int *mes, int *dia);
void calculaDomingoCarnaval(int ano, int *mes, int *dia);

#endif
