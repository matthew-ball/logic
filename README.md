# A Simple Propositional SAT Solver

## Expression
* An `expression_type` is one of the following: a `VARIABLE`, `NEGATION`, `CONJUNCTION`, `DISJUNCTION`, or `IMPLICATION`.
* An `expression_value` is one of the following: `FALSE`, or `TRUE`.
* Each `expression` has a `type`.
* Each `VARIABLE` also has a `name` and a `value`.
* `NEGATION` is a unary operator, and has a single accessible child expression.
* `CONJUNCTION`, `DISJUNCTION`, and `IMPLICATION` are binary operators, and have dual accessible child expressions.

## Environment
* An `environment` is just a linked-list containing `expression` values.

## Evaluate

## Reader

## Example
