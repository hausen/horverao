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

/* Algoritmo de Meeus/Jones/Butcher */
void calculaPascoa(int *dia, int *mes, int ano) {
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
int diaDaSemana(int dia, int mes, int ano) {
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

void calculaDomingoCarnaval(int *dia, int *mes, int ano) {
  calculaPascoa(dia, mes, ano);
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

int ehDomingoDeCarnaval(int dia, int mes, int ano) {
  int diaCarnaval, mesCarnaval, anoCarnaval = ano;
  calculaDomingoCarnaval(&diaCarnaval, &mesCarnaval, anoCarnaval);
  return (dia == diaCarnaval && mes == mesCarnaval && ano == anoCarnaval);
}

int terceiroDomingo(int mes, int ano) {
  int primeiroDomingo = 1;
  int dds = diaDaSemana(primeiroDomingo, mes, ano);
  if (dds != 0) {
    primeiroDomingo += (7 - dds);
  }
  return primeiroDomingo + 14;
}

#define TRUE 1
#define FALSE 0

int ehHorarioVerao(int dia, int mes, int ano, int dds) {
  if (ano < 2009) return -1;
  if (mes == 11 || mes == 12 || mes == 1) {
    return TRUE;
  }
  if (mes == 10) {
    if (dia < 15) return FALSE;
    if (dia >= 21) return TRUE;
    /* vai para o domingo anterior */
    if (dia-dds >= 15 && dia-dds < 21)
      return TRUE;
    else
      return FALSE;
  }
  if (mes == 2) {
    int diaFim;
    if (dia < 15) return TRUE;
    diaFim = terceiroDomingo(mes, ano);
    if (ehDomingoDeCarnaval(diaFim, mes, ano)) {
      diaFim += 7;
    } 
    if (dia >= diaFim)
      return FALSE;
    else
      return TRUE;
  }
  return FALSE;
}
