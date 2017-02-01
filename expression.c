#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"

expression_t *variable(const char *name, expression_value value) {
  variable_expression_t *ptr = malloc(sizeof(*ptr));

  MALLOC_CHECK(ptr);
  ptr->type = VARIABLE;
  ptr->value = value;
  ptr->name = malloc(strlen(name) + 1);
  MALLOC_CHECK(ptr->name);
  strcpy(ptr->name, name);

  return (expression_t *)ptr;
}

expression_t *negation(expression_t *left) {
  negation_expression_t *ptr = malloc(sizeof(*ptr));

  MALLOC_CHECK(ptr);
  ptr->type = NEGATION;
  ptr->left = left;

  return (expression_t *)ptr;
}

expression_t *conjunction(expression_t *left, expression_t *right) {
  conjunction_expression_t *ptr = malloc(sizeof(*ptr));

  MALLOC_CHECK(ptr);
  ptr->type = CONJUNCTION;
  ptr->left = left;
  ptr->right = right;

  return (expression_t *)ptr;
}

expression_t *disjunction(expression_t *left, expression_t *right) {
  disjunction_expression_t *ptr = malloc(sizeof(*ptr));

  MALLOC_CHECK(ptr);
  ptr->type = DISJUNCTION;
  ptr->left = left;
  ptr->right = right;

  return (expression_t *)ptr;
}

expression_t *implication(expression_t *left, expression_t *right) {
  implication_expression_t *ptr = malloc(sizeof(*ptr));

  MALLOC_CHECK(ptr);
  ptr->type = IMPLICATION;
  ptr->left = left;
  ptr->right = right;

  return (expression_t *)ptr;
}

void print_expression(const expression_t *expression) {
  switch (expression->type) {
  case VARIABLE:
	printf("%s", VARIABLE_NAME(expression)); break;
  case NEGATION:
	printf("¬"); print_expression(NEGATION(expression)); break;
  case CONJUNCTION:
	printf("("); print_expression(CONJUNCTION_LEFT(expression)); printf(" ∧ "); print_expression(CONJUNCTION_RIGHT(expression)); printf(")"); break;
  case DISJUNCTION:
	printf("("); print_expression(DISJUNCTION_LEFT(expression)); printf(" ∨ "); print_expression(DISJUNCTION_RIGHT(expression)); printf(")"); break;
  case IMPLICATION:
	printf("("); print_expression(IMPLICATION_LEFT(expression)); printf(" → "); print_expression(IMPLICATION_RIGHT(expression)); printf(")"); break;
  default:
	return;
  }
}

expression_value equal_expressions(const expression_t *exp1, const expression_t *exp2) {
  if (IS_VARIABLE(exp1) && IS_VARIABLE(exp2)) {
  	if (strcmp(VARIABLE_NAME(exp1), VARIABLE_NAME(exp2)) == 0) {
  	  return TRUE;
  	}
  } else if (IS_NEGATION(exp1) && IS_NEGATION(exp2)) {
  	return equal_expressions(NEGATION(exp1), NEGATION(exp2));
  } else if (IS_CONJUNCTION(exp1) && IS_CONJUNCTION(exp2)) {
  	return equal_expressions(CONJUNCTION_LEFT(exp1), CONJUNCTION_LEFT(exp2)) && equal_expressions(CONJUNCTION_RIGHT(exp1), CONJUNCTION_RIGHT(exp2));
  } else if (IS_DISJUNCTION(exp1) && IS_DISJUNCTION(exp2)) {
  	return equal_expressions(DISJUNCTION_LEFT(exp1), DISJUNCTION_LEFT(exp2)) && equal_expressions(DISJUNCTION_RIGHT(exp1), DISJUNCTION_RIGHT(exp2));
  } else if (IS_IMPLICATION(exp1) && IS_IMPLICATION(exp2)) {
  	return equal_expressions(IMPLICATION_LEFT(exp1), IMPLICATION_LEFT(exp2)) && equal_expressions(IMPLICATION_RIGHT(exp1), IMPLICATION_RIGHT(exp2));
  }

  return FALSE;
}
