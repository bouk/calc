#include <calc/eval.h>
#include <stdio.h>

static calc_value
evaluate_binary(calc_binary_node_t * node)
{
    calc_value left = calc_evaluate(node->left);
    calc_value right = calc_evaluate(node->right);

    switch(node->base.type)
    {
        case CALC_NODE_ADD:
            if(left.type == CALC_INTEGER && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_INTEGER, .val.i = (left.val.i + right.val.i)};
            }
            else if(left.type == CALC_INTEGER && right.type == CALC_FLOAT)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.i + right.val.f)};
            }
            else if(left.type == CALC_FLOAT && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f= (left.val.f + right.val.i)};
            }
            return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.f + right.val.f)};
            break;
        case CALC_NODE_SUBTRACT:
            if(left.type == CALC_INTEGER && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_INTEGER, .val.i = (left.val.i - right.val.i)};
            }
            else if(left.type == CALC_INTEGER && right.type == CALC_FLOAT)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.i - right.val.f)};
            }
            else if(left.type == CALC_FLOAT && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f= (left.val.f - right.val.i)};
            }
            return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.f - right.val.f)};
            break;
        case CALC_NODE_MULTIPLY:
            if(left.type == CALC_INTEGER && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_INTEGER, .val.i = (left.val.i * right.val.i)};
            }
            else if(left.type == CALC_INTEGER && right.type == CALC_FLOAT)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.i * right.val.f)};
            }
            else if(left.type == CALC_FLOAT && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f= (left.val.f * right.val.i)};
            }
            return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.f * right.val.f)};
            break;
        case CALC_NODE_DIVIDE:
            if(left.type == CALC_INTEGER && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_INTEGER, .val.i = (left.val.i / right.val.i)};
            }
            else if(left.type == CALC_INTEGER && right.type == CALC_FLOAT)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.i / right.val.f)};
            }
            else if(left.type == CALC_FLOAT && right.type == CALC_INTEGER)
            {
                return (calc_value) {.type = CALC_FLOAT, .val.f= (left.val.f / right.val.i)};
            }
            return (calc_value) {.type = CALC_FLOAT, .val.f = (left.val.f / right.val.f)};
            break;
        default:
            return (calc_value) {.type = CALC_INTEGER, .val.i = 0};
    }
}

calc_value
calc_evaluate(calc_base_node_t * node)
{
    switch(node->type)
    {
        case CALC_NODE_ADD:
        case CALC_NODE_SUBTRACT:
        case CALC_NODE_MULTIPLY:
        case CALC_NODE_DIVIDE:
            return evaluate_binary((calc_binary_node_t*)node);
            break;
        case CALC_NODE_VALUE:
            return ((calc_value_node_t*)node)->value;
            break;
        default:
            return (calc_value) {.type = CALC_INTEGER, .val.i = 0};
    }
}
