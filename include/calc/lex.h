#include <stdbool.h>
#include <stddef.h>

#ifndef CALC_LEX_H
#define CALC_LEX_H

typedef enum {
    CALC_TOKEN_LPAREN,
    CALC_TOKEN_RPAREN,
    CALC_TOKEN_PLUS,
    CALC_TOKEN_MINUS,
    CALC_TOKEN_INTEGER,
    CALC_TOKEN_ASTERISK,
    CALC_TOKEN_SLASH,
    CALC_TOKEN_FLOAT,
    CALC_TOKEN_END
} calc_token_type_t;

typedef struct {
    calc_token_type_t type;
    int line;
    union {
        long long i;
        double f;
    } val;
} calc_token_t;

typedef struct {
    bool failed;
    calc_token_t * tokens;
    size_t count;
    size_t cap;
} calc_lex_state_t;

calc_token_t *
calc_lex_string(const char *);

calc_token_t *
calc_lex_file(const char *);

#endif
