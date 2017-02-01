#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

typedef struct _environment_t environment_t;

typedef struct _environment_t {
  expression_t *value;
  size_t count;
  environment_t *next;
} environment_t;

extern expression_t *t;
extern expression_t *f;

expression_t *search_environment(expression_t *exp, const environment_t *env);
void add_to_environment(expression_t *exp, environment_t **env);
expression_t *remove_from_environment(environment_t **env);
environment_t *init_environment();
void print_environment(const environment_t *env);

#endif
