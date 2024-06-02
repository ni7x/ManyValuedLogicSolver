#ifndef PARSER_PARAMS_H
#define PARSER_PARAMS_H

typedef struct {
    float (*map_function)(char);
    float *result;
} ParserParams;

#endif