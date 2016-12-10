#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "environment.h"

void add_to_environment(expression *exp, environment **env) {
  environment *ptr = malloc(sizeof(environment));

  MALLOC_CHECK(ptr);

  ptr->value = exp;
  ptr->next = *env;

  *env = ptr;
}

expression *remove_from_environment(environment **env) {
  expression *value = NULL;
  environment *ptr = NULL;

  if ((*env)->value == NULL) {
	return NULL;
  }

  ptr = (*env)->next;
  value = (*env)->value;

  free(*env);
  *env = ptr;

  return value;
}

expression *search_environment(expression *exp, const environment *env) {
  while ((env)->value != NULL) {
	if (equal_expressions(exp, (env)->value)) {
	  return (env)->value;
	}

	env = (env)->next;
  }

  return NULL;
}

environment *init_environment() {
  environment *ptr = malloc(sizeof(environment));

  MALLOC_CHECK(ptr);

  expression *t = variable("#T", TRUE);
  expression *f = variable("#F", FALSE);

  add_to_environment(t, &ptr);
  add_to_environment(f, &ptr);

  return ptr;
}

void print_environment(const environment *env) {
  while ((env)->value != NULL) {
	print_expression((env)->value);
	printf("\n");
	env = (env)->next;
  }
}
