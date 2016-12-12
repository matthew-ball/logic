#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

typedef struct node {
  expression *value;
  int count;
  struct node *next;
} environment;

extern expression *t;
extern expression *f;

expression *search_environment(expression *exp, const environment *env);
void add_to_environment(expression *exp, environment **env);
expression *remove_from_environment(environment **env);
environment *init_environment();
void print_environment(const environment *env);

#endif
