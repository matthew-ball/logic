#ifndef READER_H
#define READER_H

#define MAX_BUFFER 1024

expression *next_token(FILE *input);
expression *read_expression(FILE *input);

#endif
