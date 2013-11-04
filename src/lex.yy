%{
    #include <calc/lex.h>
%}

%option noyywrap yylineno reentrant nounistd never-interactive stack
%option extra-type="calc_lex_state_t*"

%{
    #define ADD_TOKEN(token_type) do {\
        if (yyextra->count + 1 >= yyextra->cap)\
        {\
            yyextra->cap *= 2;\
            yyextra->tokens = (calc_token_t*)realloc(yyextra->tokens, sizeof(calc_token_t)*yyextra->cap);\
        }\
        yyextra->tokens[yyextra->count].type = token_type;\
        yyextra->tokens[yyextra->count].line = yylineno;\
        yyextra->count++;\
    } while (0)

    #define ADD_INTEGER_TOKEN(int_value) do {\
        ADD_TOKEN(CALC_TOKEN_INTEGER);\
        yyextra->tokens[yyextra->count - 1].val.i = int_value;\
    } while (0)

    #define ADD_FLOAT_TOKEN(float_value) do {\
        ADD_TOKEN(CALC_TOKEN_FLOAT);\
        yyextra->tokens[yyextra->count - 1].val.f = float_value;\
    } while (0)
%}

DIGIT    [0-9]
SIGN     [+-]
EXPONENT [eE]{SIGN}?{DIGIT}+

%%

<INITIAL>{SIGN}?{DIGIT}*"."{DIGIT}+{EXPONENT}? { ADD_FLOAT_TOKEN(atof(yytext)); }
<INITIAL>{SIGN}?{DIGIT}+ { ADD_INTEGER_TOKEN(atoll(yytext)); }
<INITIAL>"(" { ADD_TOKEN(CALC_TOKEN_LPAREN); }
<INITIAL>")" { ADD_TOKEN(CALC_TOKEN_RPAREN); }
<INITIAL>"+" { ADD_TOKEN(CALC_TOKEN_PLUS); }
<INITIAL>"-" { ADD_TOKEN(CALC_TOKEN_MINUS); }
<INITIAL>"*" { ADD_TOKEN(CALC_TOKEN_ASTERISK); }
<INITIAL>"/" { ADD_TOKEN(CALC_TOKEN_SLASH); }
<INITIAL>[ \n\t] { /* Ignore whitespace */ }
<INITIAL>. {
    yyextra->failed = true;
    printf("Unexpected character '%s' on line %d\n", yytext, yylineno);
}

%%

calc_token_t *
calc_lex_file(const char * filename)
{
    FILE * in;
    calc_lex_state_t state;

    state.failed = false;
    state.cap = 4;
    state.count = 0;
    state.tokens = (calc_token_t*)malloc(sizeof(calc_token_t)*state.cap);

    in = fopen(filename, "r");
    if(!in)
    {
        free(state.tokens);
        return NULL;
    }

    yyscan_t scanner;
    struct yyguts_t * yyg = (struct yyguts_t*)scanner;

    yylex_init_extra(&state, &scanner);
    yyset_in(in, scanner);
    yylex(scanner);
    yylex_destroy(scanner);
    fclose(in);

    state.tokens[state.count].type = CALC_TOKEN_END;
    if(state.failed)
    {
        return NULL;
    }

    return state.tokens;
}
