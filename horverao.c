/*
   horverao.c
   Copyright (c) 2015 Rodrigo Hausen

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.
   If not, see <http://www.gnu.org/licenses/lgpl-3.0.txt>.
 */
#include "horverao.h"

#ifndef NULL
#define NULL 0x0
#endif

/* Algoritmo de Meeus/Jones/Butcher */
void calculaPascoa(int ano, int *mes, int *dia) {
  int a = ano % 19;
  int b = ano / 100;
  int c = ano % 100;
  int d = b / 4;
  int e = b % 4;
  int f = (b + 8) / 25;
  int g = (b - f + 1) / 3;
  int h = (19 * a + b - d - g + 15) % 30;
  int i =  c / 4;
  int k = c % 4;
  int l = (32 + 2 * e + 2 * i - h - k) % 7;
  int m = (a + 11 * h + 22 * l) / 451;
  *mes = (h + l - 7 * m + 114) / 31;
  *dia = ((h + l - 7 * m + 114) % 31) + 1;
}

/* congruência de Zeller
   dia da semana: 0 = dom, 1 = seg, ..., 6 = sáb */
DiaDaSemana_t diaDaSemana(int ano, int mes, int dia) {
  int k, j;
  if (mes < 3) {
    mes += 12;
    --ano;
  }
  k = ano % 100;
  j = ano / 100;
  dia = (dia + 13*(mes+1)/5 + k + k/4 + j/100 - 2*j + 4) % 7;
  if (dia < 0) {
    return 7 + dia;
  } else {
    return dia;
  }
}

int bissexto(int ano) {
  return (ano % 400 == 0) || (ano % 4 == 0 && ano % 100 != 0);
}

void calculaDomingoCarnaval(int ano, int *mes, int *dia) {
  calculaPascoa(ano, mes, dia);
  if (*mes == 4) {
    if (*dia >= 19) {
      *mes = 3; *dia -= 18;
    } else {
      *mes = 2;
      *dia += 10 + bissexto(ano);
    }
  } else {
    *mes = 2;
    *dia -= 21 - bissexto(ano);
  }
}

int ehDomingoDeCarnaval(int ano, int mes, int dia) {
  int diaCarnaval, mesCarnaval, anoCarnaval = ano;
  calculaDomingoCarnaval(anoCarnaval, &mesCarnaval, &diaCarnaval);
  return (dia == diaCarnaval && mes == mesCarnaval && ano == anoCarnaval);
}

int terceiroDomingo(int ano, int mes) {
  int primeiroDomingo = 1;
  int dds = diaDaSemana(ano, mes, primeiroDomingo);
  if (dds != 0) {
    primeiroDomingo += (7 - dds);
  }
  return primeiroDomingo + 14;
}

void inicioHorarioVerao(int ano, int *mes, int *dia) {
    *mes = 10;
    *dia = terceiroDomingo(ano, *mes);
}

void fimHorarioVerao(int ano, int *mes, int *dia) {
    *mes = 2;
    *dia = terceiroDomingo(ano, *mes);
    if (ehDomingoDeCarnaval(ano, *mes, *dia)) {
      *dia += 7;
    }
}

Horario_t horario(int ano, int mes, int dia) {
  if (ano < 2009) return Indefinido;
  if (mes == 11 || mes == 12 || mes == 1) {
    return HorarioDeVerao;
  }
  if (mes == 10) {
    int diaInicio, mesInicio;
    inicioHorarioVerao(ano, &mesInicio, &diaInicio);
    if (dia < diaInicio) return Normal;
    return HorarioDeVerao;
  }
  if (mes == 2) {
    int diaFim, mesFim;
    fimHorarioVerao(ano, &mesFim, &diaFim);
    if (dia == diaFim - 1) return UltimoDiaHorarioDeVerao;
    if (dia < diaFim) return HorarioDeVerao;
    return Normal;
  }
  return Normal;
}

int diasDoMes(int ano, int mes) {
  switch (mes) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31;
    case 2:
      if (bissexto(ano))
        return 29;
      else
        return 28;
    case 4: case 6: case 9: case 11:
      return 30;
    default:
      return -1;
  }
}

void adicionaHoras(int *ano, int *mes, int *dia, int *hora,
                   DiaDaSemana_t *dds, int horas) {
  *hora = *hora + horas;
  if (*hora < 0) {
    *hora += 24;
    *dia -= 1;
    if (dds != NULL)
      *dds = (*dds > 0) ? *dds - 1 : 6;
    if (*dia == 0) {
      *mes -= 1;
      if (*mes == 0) {
        *mes = 12;
        *ano -= 1;
      }
      *dia = diasDoMes(*ano, *mes);
    }
  } else if (*hora >= 24) {
    *hora -= 24;
    *dia += 1;
    if (dds != NULL)
      *dds = (*dds < 6) ? *dds + 1 : 0;
    if (*dia > diasDoMes(*ano, *mes)) {
      *mes += 1;
      if (*mes == 13) {
        *mes = 1;
        *ano += 1;
      }
      *dia = 1;
    }
  }
}

void utcParaHorarioLocal(int *ano, int *mes, int *dia, int *hora,
                         DiaDaSemana_t *dds, Horario_t *isdst,
                         Fuso_t fuso, Regra_t regra) {
  int dst;
  adicionaHoras(ano, mes, dia, hora, dds, fuso);
  if (regra == NuncaEntraEmHorarioDeVerao) {
    if (isdst != NULL)
      *isdst = Normal;
    return;
  }
  dst = horario(*ano, *mes, *dia);
  if (isdst != NULL) {
    *isdst = dst;
  }
  if (dst > 0) {
    if (dst == UltimoDiaHorarioDeVerao) {
      if (*hora == 23) {
        if (isdst != NULL)
          *isdst = Normal;
        return;
      }
    }
    adicionaHoras(ano, mes, dia, hora, dds, 1);
  }
}
