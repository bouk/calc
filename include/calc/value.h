#ifndef CALC_VALUE_H
#define CALC_VALUE_H

typedef struct {
    enum {
        CALC_FLOAT,
        CALC_INTEGER
    } type;
    union {
        long long i;
        double f;
    } val;
} calc_value;

#endif
