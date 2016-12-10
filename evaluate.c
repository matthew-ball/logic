#include "expression.h"
#include "environment.h"
#include "evaluate.h"

expression *evaluate_expression(expression *exp, environment *env) {
  add_to_environment(exp, &env);

  return exp;
}
