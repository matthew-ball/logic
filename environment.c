#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "environment.h"

expression_t *search_environment(expression_t *exp, const environment_t *env) {
  while ((env)->value != NULL) {
	if (equal_expressions(exp, (env)->value)) {
	  return (env)->value;
	}

	env = (env)->next;
  }

  return NULL;
}

void add_to_environment(expression_t *exp, environment_t **env) {
  if (search_environment(exp, (*env)) == NULL) {
	environment_t *ptr = malloc(sizeof(*ptr));

	MALLOC_CHECK(ptr);

	ptr->value = exp;
	ptr->next = *env;

	ptr->count = (*env)->count + 1;

	*env = ptr;
  }
}

expression_t *remove_from_environment(environment_t **env) {
  expression_t *value = NULL;
  environment_t *ptr = NULL;

  if ((*env)->value == NULL) {
	return NULL;
  }

  ptr = (*env)->next;
  value = (*env)->value;

  (*env)->count--;

  free(*env);
  *env = ptr;

  return value;
}

expression_t *t;
expression_t *f;

environment_t *init_environment() {
  environment_t *ptr = malloc(sizeof(*ptr));

  MALLOC_CHECK(ptr);

  ptr->count = 0;

  t = variable("#T", TRUE);
  f = variable("#F", FALSE);

  add_to_environment(t, &ptr);
  add_to_environment(f, &ptr);

  return ptr;
}

void print_environment(const environment_t *env) {
  printf("{");

  while ((env)->value != NULL) {
	print_expression((env)->value);
	env = (env)->next;

	if ((env)->next != NULL) {
	  printf(", ");
	}
  }

  printf("}");
}
