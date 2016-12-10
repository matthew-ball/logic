#include "expression.h"
#include "environment.h"
#include "evaluate.h"

// https://en.wikipedia.org/wiki/Propositional_calculus#Basic_and_derived_argument_forms
expression *simplify_expression(expression *exp) {
  if (IS_NEGATION(exp) && IS_DISJUNCTION(NEGATION(exp))) { /* de morgan's disjunction */
	return conjunction(negation(DISJUNCTION_LEFT(NEGATION(exp))), negation(DISJUNCTION_RIGHT(NEGATION(exp))));
  } else if (IS_NEGATION(exp) && IS_CONJUNCTION(NEGATION(exp))) { /* de morgan's conjunction */
	return disjunction(negation(CONJUNCTION_LEFT(NEGATION(exp))), negation(CONJUNCTION_RIGHT(NEGATION(exp))));
  } else if (IS_NEGATION(exp) && IS_NEGATION(NEGATION(exp))) { /* double negation */
	return simplify_expression(NEGATION(NEGATION(exp)));
  } else if (IS_CONJUNCTION(exp)) {
	if (equal_expressions(simplify_expression(CONJUNCTION_LEFT(exp)), /* same conjuncts */
						  simplify_expression(CONJUNCTION_RIGHT(exp)))) {
	  return simplify_expression(CONJUNCTION_LEFT(exp));
	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /* modus ponens */
			   equal_expressions(simplify_expression(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))),
								 simplify_expression(CONJUNCTION_RIGHT(exp)))) {
	  return simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp)));
	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /* modus tollens */
			   IS_NEGATION(CONJUNCTION_RIGHT(exp)) &&
			   equal_expressions(simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))),
								 simplify_expression(NEGATION(CONJUNCTION_RIGHT(exp))))) {
	  return negation(simplify_expression(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))));
	} else if (IS_IMPLICATION(CONJUNCTION_LEFT(exp)) && /* hypothetical syllogism */
			   IS_IMPLICATION(CONJUNCTION_RIGHT(exp)) &&
			   equal_expressions(simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_LEFT(exp))),
								 simplify_expression(IMPLICATION_LEFT(CONJUNCTION_RIGHT(exp))))) {
	  return implication(simplify_expression(IMPLICATION_LEFT(CONJUNCTION_LEFT(exp))),
						 simplify_expression(IMPLICATION_RIGHT(CONJUNCTION_RIGHT(exp))));
	} else if (IS_DISJUNCTION(CONJUNCTION_LEFT(exp)) && /* disjunctive syllogism */
			   IS_NEGATION(CONJUNCTION_RIGHT(exp)) &&
			   equal_expressions(simplify_expression(DISJUNCTION_LEFT(CONJUNCTION_LEFT(exp))),
								 simplify_expression(NEGATION(CONJUNCTION_RIGHT(exp))))) {
	  return simplify_expression(DISJUNCTION_RIGHT(CONJUNCTION_LEFT(exp)));
	}
  } else if (IS_DISJUNCTION(exp)) {
	if (equal_expressions(simplify_expression(DISJUNCTION_LEFT(exp)), /* same disjuncts */
						  simplify_expression(DISJUNCTION_RIGHT(exp)))) {
	  return simplify_expression(DISJUNCTION_LEFT(exp));
	}
  } else if (IS_IMPLICATION(exp)) {
	if (equal_expressions(simplify_expression(IMPLICATION_LEFT(exp)), /* same antecedent and consequent */
						  simplify_expression(IMPLICATION_RIGHT(exp)))) {
	  return simplify_expression(IMPLICATION_LEFT(exp));
	}
  }

  return exp;
}

expression *evaluate_expression(expression *exp, environment **env) {
  add_to_environment(exp, env);

  return simplify_expression(exp);
}
