#ifndef READER_H
#define READER_H

#define MAX_BUFFER 1024

expression_t *next_token(FILE *input);
expression_t *read_expression(FILE *input);

#endif
