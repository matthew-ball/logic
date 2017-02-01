#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "expression.h"
#include "environment.h"
#include "evaluate.h"

expression_t *cnf(expression_t *exp) {
  if (IS_NEGATION(exp) && IS_NEGATION(NEGATION(exp))) { /* double negation */
	return cnf(NEGATION(NEGATION(exp)));
  } else if (IS_NEGATION(exp) && IS_CONJUNCTION(NEGATION(exp))) { /* de morgan's conjunction */
  	return disjunction(cnf(negation(CONJUNCTION_LEFT(NEGATION(exp)))), cnf(negation(CONJUNCTION_RIGHT(NEGATION(exp)))));
  } else if (IS_NEGATION(exp) && IS_DISJUNCTION(NEGATION(exp))) { /* de morgan's disjunction */
  	return conjunction(cnf(negation(DISJUNCTION_LEFT(NEGATION(exp)))), cnf(negation(DISJUNCTION_RIGHT(NEGATION(exp)))));
  } else if (IS_NEGATION(exp) && !IS_VARIABLE(NEGATION(exp))) {
	return cnf(negation(cnf(NEGATION(exp))));
  } if (IS_CONJUNCTION(exp)) { /* conjunction */
	return conjunction(cnf(CONJUNCTION_LEFT(exp)), cnf(CONJUNCTION_RIGHT(exp)));
  } else if (IS_DISJUNCTION(exp)) { /* disjunction */
	return disjunction(cnf(DISJUNCTION_LEFT(exp)), cnf(DISJUNCTION_RIGHT(exp)));
  } else if (IS_DISJUNCTION(exp) && IS_CONJUNCTION(DISJUNCTION_RIGHT(exp))) { /* distribution over disjunction (right) */
	return conjunction(disjunction(cnf(DISJUNCTION_LEFT(exp)), cnf(CONJUNCTION_LEFT(DISJUNCTION_RIGHT(exp)))),
					   disjunction(cnf(DISJUNCTION_LEFT(exp)), cnf(CONJUNCTION_RIGHT(DISJUNCTION_RIGHT(exp)))));
  } else if (IS_IMPLICATION(exp)) { /* implication */
	return disjunction(cnf(negation(IMPLICATION_LEFT(exp))), cnf(IMPLICATION_RIGHT(exp)));
  }

  return exp;
}

expression_t *unit_propagate(expression_t *unit, expression_t *exp) {
  // if a clause is a unit clause (i.e. it contains only a single unassigned literal) this clause can only be satisfied by assigning the necessary value to make this literal true
  return exp;
}

expression_t *pure_literal_assign(expression_t *unit, expression_t *exp) {
  // if a propositional variable occurs with only one polarity in the formula, it is called pure
  // pure literals can always be assigned in a way that makes all clauses containing them true
  return exp;
}

void filter_literals(expression_t *exp, environment_t **env) {
  if (IS_NEGATION(exp) && !IS_VARIABLE(NEGATION(exp))) {
	filter_literals(NEGATION(exp), env);
  } else if (IS_CONJUNCTION(exp)) {
	filter_literals(CONJUNCTION_LEFT(exp), env);
	filter_literals(CONJUNCTION_RIGHT(exp), env);
  } else if (IS_DISJUNCTION(exp)) {
	filter_literals(DISJUNCTION_LEFT(exp), env);
	filter_literals(DISJUNCTION_RIGHT(exp), env);
  } else if (IS_IMPLICATION(exp)) {
	filter_literals(IMPLICATION_LEFT(exp), env);
	filter_literals(IMPLICATION_RIGHT(exp), env);
  } else if (IS_LITERAL(exp)) {
	add_to_environment(exp, env);
  }
}

environment_t *collect_literals(expression_t *exp) {
  environment_t *env = malloc(sizeof(*env));

  MALLOC_CHECK(env);
  env->count = 0;

  filter_literals(exp, &env);

  return env;
}

expression_t *choose_literal(environment_t *env) {
  srand(time(NULL));
  size_t random = rand() % env->count, i;

  for (i = 0; i < random; i++) {
	env = env->next;
  }

  return env->value;
}

int consistent_literals(expression_t *exp) {
  //environment *literals = collect_literals(exp);

  return 0;
}

int contains_empty_clause(expression_t *exp) {
  return 0;
}

// https://en.wikipedia.org/wiki/DPLL_algorithm
expression_t *dpll(expression_t *exp, environment_t *env) {
  //printf("Environment: "); print_environment(env); printf("\n");
  printf("Expression: "); print_expression(exp); printf("\n");

  //exp = simplify(exp);
  //printf("- Simplified: "); print_expression(exp); printf("\n");

  exp = cnf(exp);
  printf("- Conjunctive Normal Form: "); print_expression(exp); printf("\n");

  environment_t *literals = collect_literals(exp);

  printf("Literals: "); print_environment(literals); printf("\n");

  if (consistent_literals(exp)) {
  	return t;
  } else if (contains_empty_clause(exp)) {
  	return f;
  }

  /* for every unit clause unit in exp: exp <- unit_propagate(unit, exp); */
  /* for every literal l (that occurs pure in exp): exp <- pure_literal_assign(l, exp); */

  /* expression *literal = choose_literal(literals); */

  /* return dpll(conjunction(exp, literal)) || dpll(conjunction(exp, negation(literal))); */

  return exp;
}

expression_t *evaluate_expression(expression_t *exp, environment_t **env) {
  add_to_environment(exp, env);

  return dpll(exp, *env);
}

/* // https://en.wikipedia.org/wiki/Propositional_calculus#Basic_and_derived_argument_forms */
expression_t *simplify(expression_t *exp) {
  if (IS_NEGATION(exp) && IS_DISJUNCTION(NEGATION(exp))) { /* de morgan's disjunction */
	return conjunction(negation(simplify(DISJUNCTION_LEFT(NEGATION(exp)))), negation(simplify(DISJUNCTION_RIGHT(NEGATION(exp)))));
  } else if (IS_NEGATION(exp) && IS_CONJUNCTION(NEGATION(exp))) { /* de morgan's conjunction */
	return disjunction(negation(CONJUNCTION_LEFT(NEGATION(exp))), negation(CONJUNCTION_RIGHT(NEGATION(exp))));
  } else if (IS_NEGATION(exp) && IS_NEGATION(NEGATION(exp))) { /* double negation */
	return simplify(NEGATION(NEGATION(exp)));
  } else if (IS_CONJUNCTION(exp)) {
	if (equal_expressions(simplify(CONJUNCTION_LEFT(exp)), /* same conjuncts */
						  simplify(CONJUNCTION_RIGHT(exp)))) {
	  return simplify(CONJUNCTION_LEFT(exp));
	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /* modus ponens */
			   equal_expressions(simplify(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))),
								 simplify(CONJUNCTION_RIGHT(exp)))) {
	  return simplify(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp)));
	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /* modus tollens */
			   IS_NEGATION(CONJUNCTION_RIGHT(exp)) &&
			   equal_expressions(simplify(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))),
								 simplify(NEGATION(CONJUNCTION_RIGHT(exp))))) {
	  return negation(simplify(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))));
	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /* hypothetical syllogism */
			   IS_IMPLICATION(CONJUNCTION_RIGHT(exp)) &&
			   equal_expressions(simplify(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))),
								 simplify(IMPLICATION_LEFT(CONJUNCTION_RIGHT(exp))))) {
	  return implication(simplify(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))),
						 simplify(IMPLICATION_RIGHT(CONJUNCTION_RIGHT(exp))));
	} else if (IS_DISJUNCTION(CONJUNCTION_LEFT(exp)) && /* disjunctive syllogism */
			   IS_NEGATION(CONJUNCTION_RIGHT(exp)) &&
			   equal_expressions(simplify(DISJUNCTION_LEFT(CONJUNCTION_LEFT(exp))),
								 simplify(NEGATION(CONJUNCTION_RIGHT(exp))))) {
	  return simplify(DISJUNCTION_RIGHT(CONJUNCTION_LEFT(exp)));
	}
  } else if (IS_DISJUNCTION(exp)) {
	if (equal_expressions(simplify(DISJUNCTION_LEFT(exp)), /* same disjuncts */
						  simplify(DISJUNCTION_RIGHT(exp)))) {
	  return simplify(DISJUNCTION_LEFT(exp));
	}
  } else if (IS_IMPLICATION(exp)) {
	if (equal_expressions(simplify(IMPLICATION_LEFT(exp)), /* same antecedent and consequent */
						  simplify(IMPLICATION_RIGHT(exp)))) {
	  return simplify(IMPLICATION_LEFT(exp));
	}
  }

  return exp;
}
