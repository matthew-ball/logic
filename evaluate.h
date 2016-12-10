#ifndef EVALUATE_H
#define EVALUATE_H

expression *conjunctive_normal_form(expression *exp);
expression *unit_propagate(expression *unit, expression *exp);
expression *pure_literal_assign(expression *unit, expression *exp);
expression *choose_literal(expression *exp);

expression *simplify_expression(expression *exp);
expression *evaluate_expression(expression *exp, environment **env);

#endif
