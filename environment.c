#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "environment.h"

expression *search_environment(expression *exp, const environment *env) {
  while ((env)->value != NULL) {
	if (equal_expressions(exp, (env)->value)) {
	  return (env)->value;
	}

	env = (env)->next;
  }

  return NULL;
}

void add_to_environment(expression *exp, environment **env) {
  if (search_environment(exp, (*env)) == NULL) {
	environment *ptr = malloc(sizeof(environment));

	MALLOC_CHECK(ptr);

	ptr->value = exp;
	ptr->next = *env;

	ptr->count = (*env)->count + 1;

	*env = ptr;
  }
}

expression *remove_from_environment(environment **env) {
  expression *value = NULL;
  environment *ptr = NULL;

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

expression *t;
expression *f;

environment *init_environment() {
  environment *ptr = malloc(sizeof(environment));

  MALLOC_CHECK(ptr);

  ptr->count = 0;

  t = variable("#T", TRUE);
  f = variable("#F", FALSE);

  add_to_environment(t, &ptr);
  add_to_environment(f, &ptr);

  return ptr;
}

void print_environment(const environment *env) {
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
