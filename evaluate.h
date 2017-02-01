#ifndef EVALUATE_H
#define EVALUATE_H

//extern expression *t;
//extern expression *f;

expression_t *cnf(expression_t *exp);
expression_t *unit_propagate(expression_t *unit, expression_t *exp);
expression_t *pure_literal_assign(expression_t *unit, expression_t *exp);
void filter_literals(expression_t *exp, environment_t **env);
environment_t *collect_literals(expression_t *exp);
expression_t *choose_literal(environment_t *env);
expression_t *dpll(expression_t *exp, environment_t *env);

expression_t *simplify(expression_t *exp);

expression_t *evaluate_expression(expression_t *exp, environment_t **env);

#endif
