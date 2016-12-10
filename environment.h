#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

typedef struct node {
  expression *value;
  struct node *next;
} environment;

extern expression *t;
extern expression *f;

void add_to_environment(expression *exp, environment **env);
expression *remove_from_environment(environment **env);
expression *search_environment(expression *exp, const environment *env);
environment *init_environment();
void print_environment(const environment *env);

#endif
