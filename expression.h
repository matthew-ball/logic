#ifndef EXPRESSION_H
#define EXPRESSION_H

#define MALLOC_CHECK(ptr) ({ if ((!ptr)) { fprintf(stderr, "[%s] malloc failed\n", __func__); exit(EXIT_FAILURE); } })

typedef enum { VARIABLE, NEGATION, CONJUNCTION, DISJUNCTION, IMPLICATION } expression_type;
typedef enum { FALSE, TRUE } expression_value;

typedef struct {
  expression_type type;
} expression_t;

typedef struct {
  expression_type type;
  expression_value value;
  char *name;
} variable_expression_t;

#define DEFINE_UNARY_OPERATOR(name) typedef struct { expression_type type; expression_t *left; } name##_expression_t;
#define DEFINE_BINARY_OPERATOR(name) typedef struct { expression_type type; expression_t *left; expression_t *right; } name##_expression_t;

DEFINE_UNARY_OPERATOR(negation);
DEFINE_BINARY_OPERATOR(conjunction);
DEFINE_BINARY_OPERATOR(disjunction);
DEFINE_BINARY_OPERATOR(implication);

#define IS_VARIABLE(exp) ((exp)->type == VARIABLE)
#define IS_NEGATION(exp) ((exp)->type == NEGATION)
#define IS_CONJUNCTION(exp) ((exp)->type == CONJUNCTION)
#define IS_DISJUNCTION(exp) ((exp)->type == DISJUNCTION)
#define IS_IMPLICATION(exp) ((exp)->type == IMPLICATION)
#define IS_LITERAL(exp) (IS_VARIABLE(exp) || (IS_NEGATION(exp) && IS_VARIABLE(NEGATION(exp))))

#define VARIABLE_NAME(var) (((variable_expression_t *)(var))->name)
#define VARIABLE_VALUE(var) (((variable_expression_t *)(var))->value)
#define NEGATION(exp) (((negation_expression_t *)(exp))->left)
#define CONJUNCTION_LEFT(exp) (((conjunction_expression_t *)(exp))->left)
#define CONJUNCTION_RIGHT(exp) (((conjunction_expression_t *)(exp))->right)
#define DISJUNCTION_LEFT(exp) (((disjunction_expression_t *)(exp))->left)
#define DISJUNCTION_RIGHT(exp) (((disjunction_expression_t *)(exp))->right)
#define IMPLICATION_LEFT(exp) (((implication_expression_t *)(exp))->left)
#define IMPLICATION_RIGHT(exp) (((implication_expression_t *)(exp))->right)

expression_t *variable(const char *name, expression_value value);
expression_t *negation(expression_t *left);
expression_t *conjunction(expression_t *left, expression_t *right);
expression_t *disjunction(expression_t *left, expression_t *right);
expression_t *implication(expression_t *left, expression_t *right);
void print_expression(const expression_t *exp);
expression_value equal_expressions(const expression_t *exp1, const expression_t *exp2);

#endif
