#include <stdio.h>

#include <calc/eval.h>
#include <calc/lex.h>
#include <calc/parse.h>
#include <calc/value.h>

int
main(int argc, char const *argv[])
{
    FILE * in;
    calc_token_t * tokens;
    if(argc != 2)
    {
        printf("Please give file\n");
        return 1;
    }

    in = fopen(argv[1], "r");
    if(!in)
    {
        printf("Failed to open %s\n", argv[1]);
        return 2;
    }

    tokens = calc_lex_file(argv[1]);
    if(!tokens)
    {
        printf("Failed to lex\n");
        return 3;
    }
    calc_parse_state_t * state = calc_make_parse_state(tokens);
    calc_base_node_t * root = calc_expression(state);

    if(state->failed)
    {
        printf("Parsing failed\n");
        return 4;
    }

    calc_value result = calc_evaluate(root);
    if(result.type == CALC_INTEGER)
    {
        printf("%lld\n", result.val.i);
    }
    else
        {
        printf("%lf\n", result.val.f);
    }

    return 0;
}
