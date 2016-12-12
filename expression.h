#ifndef EXPRESSION_H
#define EXPRESSION_H

#define MALLOC_CHECK(ptr) ({ if ((ptr) == NULL) { fprintf(stderr, "[%s] malloc failed\n", __func__); exit(EXIT_FAILURE); } })

typedef enum { VARIABLE, NEGATION, CONJUNCTION, DISJUNCTION, IMPLICATION } expression_type;
typedef enum { FALSE, TRUE } expression_value;

typedef struct {
  expression_type type;
} expression;

typedef struct {
  expression_type type;
  expression_value value;
  char *name;
} variable_expression;

#define DEFINE_UNARY_OPERATOR(name) typedef struct { expression_type type; expression *left; } name##_expression;
#define DEFINE_BINARY_OPERATOR(name) typedef struct { expression_type type; expression *left; expression *right; } name##_expression;

DEFINE_UNARY_OPERATOR(negation);
DEFINE_BINARY_OPERATOR(conjunction);
DEFINE_BINARY_OPERATOR(disjunction);
DEFINE_BINARY_OPERATOR(implication);

#define IS_VARIABLE(exp) ((exp)->type == VARIABLE)
#define IS_NEGATION(exp) ((exp)->type == NEGATION)
#define IS_CONJUNCTION(exp) ((exp)->type == CONJUNCTION)
#define IS_DISJUNCTION(exp) ((exp)->type == DISJUNCTION)
#define IS_IMPLICATION(exp) ((exp)->type == IMPLICATION)
#define VARIABLE_NAME(var) (((variable_expression *)(var))->name)
#define VARIABLE_VALUE(var) (((variable_expression *)(var))->value)
#define NEGATION(exp) (((negation_expression *)(exp))->left)
#define CONJUNCTION_LEFT(exp) (((conjunction_expression *)(exp))->left)
#define CONJUNCTION_RIGHT(exp) (((conjunction_expression *)(exp))->right)
#define DISJUNCTION_LEFT(exp) (((disjunction_expression *)(exp))->left)
#define DISJUNCTION_RIGHT(exp) (((disjunction_expression *)(exp))->right)
#define IMPLICATION_LEFT(exp) (((implication_expression *)(exp))->left)
#define IMPLICATION_RIGHT(exp) (((implication_expression *)(exp))->right)

expression *variable(const char *name, expression_value value);
expression *negation(expression *left);
expression *conjunction(expression *left, expression *right);
expression *disjunction(expression *left, expression *right);
expression *implication(expression *left, expression *right);
void print_expression(const expression *exp);
expression_value equal_expressions(const expression *exp1, const expression *exp2);

#endif
