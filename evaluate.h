#ifndef EVALUATE_H
#define EVALUATE_H

expression *cnf(expression *exp);
expression *unit_propagate(expression *unit, expression *exp);
expression *pure_literal_assign(expression *unit, expression *exp);
void filter_literals(expression *exp, environment **env);
environment *collect_literals(expression *exp);
expression *choose_literal(environment *env);
expression *dpll(expression *exp);

expression *simplify_expression(expression *exp);
expression *evaluate_expression(expression *exp, environment **env);

#endif
