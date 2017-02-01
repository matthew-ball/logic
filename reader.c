#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "expression.h"
#include "reader.h"

expression_t *next_token(FILE *input) {
  char buffer[MAX_BUFFER];
  int index = 0, ch = getc(input);

  while (isspace(ch)) {
	ch = getc(input);
  }

  if (ch == '\n') {
	ch = getc(input);
  }

  if (ch == EOF) {
	exit(EXIT_SUCCESS);
  }

  if (ch == ')') {
	return variable(")", FALSE);
  }

  if (ch == '(') {
	return variable("(", FALSE);
  }

  while (!isspace(ch) && ch != ')') {
	buffer[index++] = ch;
	ch = getc(input);
  }

  buffer[index++] = '\0';

  /* if (ch == ')') { */
  /* 	ungetc(ch, input); */
  /* } */

  return variable(buffer, FALSE);
}

expression_t *read_expression(FILE *input) {
  expression_t *token = next_token(input);

  if (strcmp(VARIABLE_NAME(token), ")") == 0) {
	return NULL;
  } else if (strcmp(VARIABLE_NAME(token), "(") == 0) {
	return read_expression(input);
  } else if (strcmp(VARIABLE_NAME(token), "negation") == 0) {
	return negation(read_expression(input));
  } else if (strcmp(VARIABLE_NAME(token), "conjunction") == 0) {
	return conjunction(read_expression(input), read_expression(input));
  } else if (strcmp(VARIABLE_NAME(token), "disjunction") == 0) {
	return disjunction(read_expression(input), read_expression(input));
  } else if (strcmp(VARIABLE_NAME(token), "implication") == 0) {
	return implication(read_expression(input), read_expression(input));
  } else {
	return token;
  }
}
