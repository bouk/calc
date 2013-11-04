#include <stdlib.h>
#include <stdio.h>

#include <calc/parse.h>

static calc_token_type_t
peek_type(calc_parse_state_t * p)
{
    return p->tokens[p->pos].type;
}

static calc_token_t *
next_token(calc_parse_state_t * p)
{
    return &p->tokens[p->pos++];
}

static calc_base_node_t *
make_value_node(calc_token_t * token)
{
    calc_value_node_t * node = (calc_value_node_t*)malloc(sizeof(calc_value_node_t));
    node->base.line = token->line;
    node->base.type = CALC_NODE_VALUE;
    switch(token->type)
    {
        case CALC_TOKEN_INTEGER:
            node->value = (calc_value) { .val.i = token->val.i, .type = CALC_INTEGER };
            break;
        case CALC_TOKEN_FLOAT:
            node->value = (calc_value) { .val.f = token->val.f, .type = CALC_FLOAT };
            break;
        default:
            return NULL;
    }
    return (calc_base_node_t*)node;
}

static calc_base_node_t *
make_binary_node(calc_node_type_t type, int line, calc_base_node_t * left, calc_base_node_t * right)
{
    calc_binary_node_t * node = (calc_binary_node_t*)malloc(sizeof(calc_binary_node_t));
    node->base.line = line;
    node->base.type = type;
    node->left = left;
    node->right = right;
    return (calc_base_node_t*)node;
}

static calc_base_node_t *
value_expression(calc_parse_state_t * p)
{
    calc_base_node_t * value;
    switch(peek_type(p))
    {
        case CALC_TOKEN_LPAREN:
            next_token(p);
            value = calc_expression(p);
            if(peek_type(p) != CALC_TOKEN_RPAREN)
            {
                p->failed = true;
                return NULL;
            }
            next_token(p);
            return value;
            break;
        case CALC_TOKEN_INTEGER:
        case CALC_TOKEN_FLOAT:
            return make_value_node(next_token(p));
            break;
        default:
            p->failed = true;
            return NULL;
    }
}

static calc_base_node_t *
multiply_expression(calc_parse_state_t * p)
{
    calc_base_node_t * left = value_expression(p);
    while(peek_type(p) == CALC_TOKEN_ASTERISK || peek_type(p) == CALC_TOKEN_SLASH)
    {
        calc_token_t * token = next_token(p);
        switch(token->type)
        {
            case CALC_TOKEN_ASTERISK:
                left = make_binary_node(CALC_NODE_MULTIPLY, token->line, left, value_expression(p));
                break;
            case CALC_TOKEN_SLASH:
                left = make_binary_node(CALC_NODE_DIVIDE, token->line, left, value_expression(p));
                break;
            default:
                return NULL;
                break;
        }
    }
    return left;
}

static calc_base_node_t *
add_expression(calc_parse_state_t * p)
{
    calc_base_node_t * left = multiply_expression(p);
    while(peek_type(p) == CALC_TOKEN_PLUS || peek_type(p) == CALC_TOKEN_MINUS)
    {
        calc_token_t * token = next_token(p);
        switch(token->type)
        {
            case CALC_TOKEN_PLUS:
                left = make_binary_node(CALC_NODE_ADD, token->line, left, multiply_expression(p));
                break;
            case CALC_TOKEN_MINUS:
                left = make_binary_node(CALC_NODE_SUBTRACT, token->line, left, multiply_expression(p));
                break;
            default:
                return NULL;
                break;
        }
    }
    return left;
}

calc_base_node_t *
calc_expression(calc_parse_state_t * p)
{
    return add_expression(p);
}

calc_parse_state_t *
calc_make_parse_state(calc_token_t * tokens)
{
    calc_parse_state_t * state = (calc_parse_state_t*)malloc(sizeof(calc_parse_state_t));
    state->failed = false;
    state->pos = 0;
    state->tokens = tokens;
    return state;
}
