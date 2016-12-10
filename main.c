#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression.h"
#include "environment.h"
#include "evaluate.h"
#include "reader.h"

void repl(FILE *input, environment *env) {
  int check = 0;

  do {
  	if (check == 0) {
  	  printf("evaluate> ");
  	}

  	expression *ptr = read_expression(input);

  	if (ptr != NULL) {
  	  print_expression(evaluate_expression(ptr, env)); printf("\n");

  	  if (check != 0) {
  		check = 0;
  	  }
  	} else {
  	  check = 1;
  	}
  } while (1);
}

int main(int argc, char *argv[]) {
  FILE *input = ((argc > 1) ? fopen(argv[1], "r") : stdin);
  environment *env = init_environment();

  repl(input, env);

  fclose(input);

  return EXIT_SUCCESS;
}
