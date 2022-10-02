#include "lvdsc.h"
#include <stdarg.h>
#include <stdio.h>
int getInt(char *str, ...) {
  va_list ap;
  int value;

  va_start(ap, str); 
  vprintf(str, ap);

  while(!scanf("%d", &value))
  {
    printf("\nDigite novamente:");
  }

  return value;
}

int enumero(char *digito) {
  // declada e define as variaveis.
  int retorno = 1;
  int i = 0;

  while (digito[i]) { // verifica se há valores não-digitos na string.
    if ((digito[i] < '0') || (digito[i] > '9')) {
      retorno = 0;
    }

    i++;
  }

  return retorno;
}


