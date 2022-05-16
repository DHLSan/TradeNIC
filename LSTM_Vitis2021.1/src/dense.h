#include <stdio.h>
#include <stdlib.h>

#define NEURONS 100

void dense(
    float h_state[NEURONS],
    float dense_w[NEURONS],
    float dense_b,
    float *dense_out
    )
{
    int i;
    float result = dense_b;
    for (i = 0; i < NEURONS; ++i) {
        result += (dense_w[i] * h_state[i]);
    }
    *dense_out = result;
    return;
}
