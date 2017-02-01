#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression.h"
#include "environment.h"
#include "evaluate.h"
#include "reader.h"

void repl(FILE *input, environment_t **env, int interactive) {
  int check = 0;

  do {
  	if (check == 0 && interactive < 2) {
  	  printf("evaluate> ");
  	}

  	expression_t *ptr = read_expression(input);

  	if (ptr != NULL) {
  	  //print_expression(ptr); printf(" = "); print_expression(evaluate_expression(ptr, env)); printf("\n");
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

  if (!input) {
	fprintf(stderr, "[%s] failed to open file\n", __func__);
	exit(EXIT_FAILURE);
  }

  environment_t *env = init_environment();

  repl(input, &env, argc);

  fclose(input);

  return EXIT_SUCCESS;
}
