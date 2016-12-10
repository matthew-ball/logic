#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"

expression *variable(const char *name, expression_value value) {
  variable_expression *ptr = malloc(sizeof(variable_expression));

  MALLOC_CHECK(ptr);

  ptr->type = VARIABLE;
  ptr->value = value;

  char *tmp = malloc(strlen(name) + 1);

  strcpy(tmp, name);
  ptr->name = tmp;

  return (expression *)ptr;
}

expression *negation(expression *left) {
  negation_expression *ptr = malloc(sizeof(negation_expression));

  MALLOC_CHECK(ptr);

  ptr->type = NEGATION;
  ptr->left = left;

  return (expression *)ptr;
}

expression *conjunction(expression *left, expression *right) {
  conjunction_expression *ptr = malloc(sizeof(conjunction_expression));

  MALLOC_CHECK(ptr);

  ptr->type = CONJUNCTION;
  ptr->left = left;
  ptr->right = right;

  return (expression *)ptr;
}

expression *disjunction(expression *left, expression *right) {
  disjunction_expression *ptr = malloc(sizeof(disjunction_expression));

  MALLOC_CHECK(ptr);

  ptr->type = DISJUNCTION;
  ptr->left = left;
  ptr->right = right;

  return (expression *)ptr;
}

expression *implication(expression *left, expression *right) {
  implication_expression *ptr = malloc(sizeof(implication_expression));

  MALLOC_CHECK(ptr);

  ptr->type = IMPLICATION;
  ptr->left = left;
  ptr->right = right;

  return (expression *)ptr;
}

void print_expression(const expression *exp) {
  if (exp == NULL) {
	return;
  } else if (IS_VARIABLE(exp)) {
	//printf("%s (%s)", VARIABLE_NAME(exp), (VARIABLE_VALUE(exp) > 2) ? "true" : "false");
	printf("%s", VARIABLE_NAME(exp));
  } else if (IS_NEGATION(exp)) {
	printf("(not "); print_expression(NEGATION(exp)); printf(")");
  } else if (IS_CONJUNCTION(exp)) {
	printf("("); print_expression(CONJUNCTION_LEFT(exp)); printf(" and "); print_expression(CONJUNCTION_RIGHT(exp)); printf(")");
  } else if (IS_DISJUNCTION(exp)) {
	printf("("); print_expression(DISJUNCTION_LEFT(exp)); printf(" or "); print_expression(DISJUNCTION_RIGHT(exp)); printf(")");
  } else if (IS_IMPLICATION(exp)) {
	printf("("); print_expression(IMPLICATION_RIGHT(exp)); printf(" if "); print_expression(IMPLICATION_LEFT(exp)); printf(")");
  }
}

expression_value equal_expressions(expression *exp1, expression *exp2) {
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
