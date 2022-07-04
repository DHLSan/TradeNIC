#include <stdio.h>
#include <stdlib.h>

#define NEURONS 100

void dense(
    float h_state[NEURONS],
    float dense_w[NEURONS],
    float dense_b,
    float dense_out[4]
    )
{
#pragma HLS INTERFACE s_axilite port = h_state bundle = BUS_A
#pragma HLS INTERFACE s_axilite port = dense_w bundle = BUS_A
#pragma HLS INTERFACE s_axilite port = dense_b bundle = BUS_A
#pragma HLS INTERFACE s_axilite port = dense_out bundle = BUS_A
#pragma HLS INTERFACE s_axilite port = return bundle = BUS_A

    float result = dense_b;
    for (int i = 0; i < NEURONS; ++i) {
		#pragma HLS PIPELINE
        result += (dense_w[i] * h_state[i]);
    }
    dense_out[0] = result;
    return;
}
