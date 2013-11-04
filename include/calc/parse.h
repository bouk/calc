#include <calc/lex.h>
#include <calc/value.h>

#ifndef CALC_PARSE_H
#define CALC_PARSE_H

typedef enum {
    CALC_NODE_ADD,
    CALC_NODE_SUBTRACT,
    CALC_NODE_MULTIPLY,
    CALC_NODE_DIVIDE,
    CALC_NODE_VALUE
} calc_node_type_t;

typedef struct {
    calc_node_type_t type;
    int line;
} calc_base_node_t;

typedef struct {
    calc_base_node_t base;
    calc_base_node_t * child;
} calc_unary_node_t;

typedef struct {
    calc_base_node_t base;
    calc_base_node_t * left;
    calc_base_node_t * right;
} calc_binary_node_t;

typedef struct {
    calc_base_node_t base;
    calc_value value;
} calc_value_node_t;

typedef struct {
    bool failed;
    calc_token_t * tokens;
    size_t pos;
} calc_parse_state_t;


calc_parse_state_t *
calc_make_parse_state(calc_token_t *);

calc_base_node_t *
calc_expression(calc_parse_state_t * ps);

#endif
