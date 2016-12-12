#include <stdio.h>
#include <stdlib.h>

#include "expression.h"
#include "environment.h"
#include "evaluate.h"

expression *cnf(expression *exp) {
  expression *ptr = exp;

  if (IS_IMPLICATION(exp)) { /* implication */
	ptr = disjunction(negation(IMPLICATION_LEFT(exp)), IMPLICATION_RIGHT(exp));
  } else if (IS_DISJUNCTION(exp) && IS_CONJUNCTION(DISJUNCTION_RIGHT(exp))) { /* disjunctions in */
	ptr = conjunction(disjunction(DISJUNCTION_LEFT(exp), CONJUNCTION_LEFT(DISJUNCTION_RIGHT(exp))), disjunction(DISJUNCTION_LEFT(exp), CONJUNCTION_RIGHT(DISJUNCTION_RIGHT(exp))));
  } else if (IS_NEGATION(exp) && IS_NEGATION(NEGATION(exp))) { /* double negation */
	ptr = NEGATION(NEGATION(exp));
  } else if (IS_NEGATION(exp) && IS_CONJUNCTION(NEGATION(exp))) { /* de morgan's conjunction */
	ptr = disjunction(negation(CONJUNCTION_LEFT(NEGATION(exp))), negation(CONJUNCTION_RIGHT(NEGATION(exp))));
  } else if (IS_NEGATION(exp) && IS_DISJUNCTION(NEGATION(exp))) { /* de morgan's disjunction */
	ptr = conjunction(negation(DISJUNCTION_LEFT(NEGATION(exp))), negation(DISJUNCTION_RIGHT(NEGATION(exp))));
  }

  printf("[%s] Conjunctive Normal Form: ", __func__); print_expression(ptr); printf("\n");

  return ptr;
}

expression *unit_propagate(expression *unit, expression *exp) {
  return exp;
}

expression *pure_literal_assign(expression *unit, expression *exp) {
  return exp;
}

void filter_literals(expression *exp, environment **env) {
  if (IS_CONJUNCTION(exp)) {
	filter_literals(CONJUNCTION_LEFT(exp), env);
	filter_literals(CONJUNCTION_RIGHT(exp), env);
  } else if (IS_DISJUNCTION(exp)) {
	filter_literals(DISJUNCTION_LEFT(exp), env);
	filter_literals(DISJUNCTION_RIGHT(exp), env);
  } else if (IS_IMPLICATION(exp)) {
	filter_literals(IMPLICATION_LEFT(exp), env);
	filter_literals(IMPLICATION_RIGHT(exp), env);
  } else if (IS_NEGATION(exp) || IS_VARIABLE(exp)) {
	add_to_environment(exp, env);
  }
}

environment *collect_literals(expression *exp) {
  environment *env = malloc(sizeof(environment));

  MALLOC_CHECK(env);
  env->count = 0;

  filter_literals(exp, &env);

  return env;
}

/* expression *choose_literal(environment *env) { */
/*   srand(time(NULL)); */
/*   int count = (rand() % env->count); */

/*   while (count > 0) { */
/* 	env = env->next; */
/* 	count--; */
/*   } */

/*   return env->value; */
/* } */

expression *dpll(expression *exp) {
  environment *env = collect_literals(cnf(exp));

  printf("[%s] Literals: ", __func__); print_environment(env); printf("\n");

  /* while (env->value != NULL) { */
  /* 	//printf("Literal: "); print_expression(env->value); printf("\n"); */
  /* 	env = env->next; */
  /* } */
  //print_expression(choose_literal(env));

  return exp;
}

/* expression *de_morgan_conjunction(expression *exp) { */
/*   if (IS_NEGATION(exp) && IS_CONJUNCTION(NEGATION(exp))) { */
/* 	return disjunction(negation(CONJUNCTION_LEFT(NEGATION(exp))), */
/* 					   negation(CONJUNCTION_RIGHT(NEGATION(exp)))); */
/*   } else { */
/* 	return exp; */
/*   } */
/* } */

/* expression *de_morgan_disjunction(expression *exp) { */
/*   if (IS_NEGATION(exp) && IS_DISJUNCTION(NEGATION(exp))) { */
/* 	return conjunction(negation(DISJUNCTION_LEFT(NEGATION(exp))), */
/* 					   negation(DISJUNCTION_RIGHT(NEGATION(exp)))); */
/*   } else { */
/* 	return exp; */
/*   } */
/* } */

/* expression *modus_ponens(expression *exp) { */
/*   if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && */
/* 	  equal_expressions(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp)), */
/* 						CONJUNCTION_RIGHT(exp))) { */
/* 	return IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp)); */
/*   } else { */
/* 	return exp; */
/*   } */
/* } */

/* expression *modus_tollens(expression *exp) { */
/*   if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && */
/* 	  IS_NEGATION(CONJUNCTION_RIGHT(exp)) && */
/* 	  equal_expressions(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp)), */
/* 						NEGATION(CONJUNCTION_RIGHT(exp)))) { */
/* 	return negation(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))); */
/*   } else { */
/* 	return exp; */
/*   } */
/* } */

/* expression *double_negation(expression *exp) { */
/*   if (IS_NEGATION(exp) && IS_NEGATION(NEGATION(exp))) { */
/* 	return NEGATION(NEGATION(exp)); */
/*   } else { */
/* 	return exp; */
/*   } */
/* } */

/* expression *hypothetical_syllogism(expression *exp); */
/* expression *disjunctive_syllogism(expression *exp); */

/* // https://en.wikipedia.org/wiki/Propositional_calculus#Basic_and_derived_argument_forms */
/* expression *simplify_expression(expression *exp) { */
/*   if (IS_NEGATION(exp) && IS_DISJUNCTION(NEGATION(exp))) { /\* de morgan's disjunction *\/ */
/* 	return conjunction(negation(simplify_expression(DISJUNCTION_LEFT(NEGATION(exp)))), negation(simplify_expression(DISJUNCTION_RIGHT(NEGATION(exp))))); */
/*   } else if (IS_NEGATION(exp) && IS_CONJUNCTION(NEGATION(exp))) { /\* de morgan's conjunction *\/ */
/* 	return disjunction(negation(CONJUNCTION_LEFT(NEGATION(exp))), negation(CONJUNCTION_RIGHT(NEGATION(exp)))); */
/*   } else if (IS_NEGATION(exp) && IS_NEGATION(NEGATION(exp))) { /\* double negation *\/ */
/* 	return simplify_expression(NEGATION(NEGATION(exp))); */
/*   } else if (IS_CONJUNCTION(exp)) { */
/* 	if (equal_expressions(simplify_expression(CONJUNCTION_LEFT(exp)), /\* same conjuncts *\/ */
/* 						  simplify_expression(CONJUNCTION_RIGHT(exp)))) { */
/* 	  return simplify_expression(CONJUNCTION_LEFT(exp)); */
/* 	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /\* modus ponens *\/ */
/* 			   equal_expressions(simplify_expression(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))), */
/* 								 simplify_expression(CONJUNCTION_RIGHT(exp)))) { */
/* 	  return simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))); */
/* 	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /\* modus tollens *\/ */
/* 			   IS_NEGATION(CONJUNCTION_RIGHT(exp)) && */
/* 			   equal_expressions(simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))), */
/* 								 simplify_expression(NEGATION(CONJUNCTION_RIGHT(exp))))) { */
/* 	  return negation(simplify_expression(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp)))); */
/* 	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /\* hypothetical syllogism *\/ */
/* 			   IS_IMPLICATION(CONJUNCTION_RIGHT(exp)) && */
/* 			   equal_expressions(simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))), */
/* 								 simplify_expression(IMPLICATION_LEFT(CONJUNCTION_RIGHT(exp))))) { */
/* 	  return implication(simplify_expression(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))), */
/* 						 simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_RIGHT(exp)))); */
/* 	} else if (IS_DISJUNCTION(CONJUNCTION_LEFT(exp)) && /\* disjunctive syllogism *\/ */
/* 			   IS_NEGATION(CONJUNCTION_RIGHT(exp)) && */
/* 			   equal_expressions(simplify_expression(DISJUNCTION_LEFT(CONJUNCTION_LEFT(exp))), */
/* 								 simplify_expression(NEGATION(CONJUNCTION_RIGHT(exp))))) { */
/* 	  return simplify_expression(DISJUNCTION_RIGHT(CONJUNCTION_LEFT(exp))); */
/* 	} */
/*   } else if (IS_DISJUNCTION(exp)) { */
/* 	if (equal_expressions(simplify_expression(DISJUNCTION_LEFT(exp)), /\* same disjuncts *\/ */
/* 						  simplify_expression(DISJUNCTION_RIGHT(exp)))) { */
/* 	  return simplify_expression(DISJUNCTION_LEFT(exp)); */
/* 	} */
/*   } else if (IS_IMPLICATION(exp)) { */
/* 	if (equal_expressions(simplify_expression(IMPLICATION_LEFT(exp)), /\* same antecedent and consequent *\/ */
/* 						  simplify_expression(IMPLICATION_RIGHT(exp)))) { */
/* 	  return simplify_expression(IMPLICATION_LEFT(exp)); */
/* 	} */
/*   } */

/*   return exp; */
/* } */

expression *evaluate_expression(expression *exp, environment **env) {
  add_to_environment(exp, env);

  return dpll(exp);
}
