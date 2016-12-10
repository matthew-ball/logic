#include "expression.h"
#include "environment.h"
#include "evaluate.h"

expression *simplify_expression(expression *exp) {
  return exp;
}

expression *evaluate_expression(expression *exp, environment *env) {
  add_to_environment(exp, &env);

  return exp;
}
