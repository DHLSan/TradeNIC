#include <stdio.h>
#include <stdlib.h>
#include <hls_math.h>
#include <ap_fixed.h>
#include "hls_exp_apfixed.h"

#define NEURONS 100
#define INPUT_DIM 2
#define TIME_STEP 50
#define TEST_X_SIZE TIME_STEP-1
#define GATES 4 //input, forget, cell, output

//typedef ap_fixed<32,3> din1_t;
typedef ap_fixed<32,4> din1_t;
/*float sigmoid(float x)
{
    float exp_val=2.7182818284590452354;
    float result;
    result = 1 / (1 + (pow(exp_val,(-x))));
    return result;
}*/

float sigmoid(din1_t x)
{
    float exp_val;
    float result;
    exp_val = exp((float) -x);
    result = 1 / (1 + exp_val);
    return result;
}

void lstm(
    float input_data[INPUT_DIM], // 49 * 2
	float kernel[2][NEURONS * GATES], //shape=(input_dim, self.units * 4)
	float recurrent_kernel[100][NEURONS * GATES], //shape=(self.units, self.units * 4)
	float bias[NEURONS * GATES],
    float lstm_out[NEURONS],
    int reset
    )
{

	#pragma HLS INTERFACE s_axilite port = input_data bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = kernel bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = recurrent_kernel bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = bias bundle=BUS_A
	#pragma HLS INTERFACE s_axilite port = lstm_out bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = reset bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = return bundle = BUS_A

    static din1_t h_state[NEURONS] = {0};
    static din1_t x_state[NEURONS] = {0};
    din1_t i_state[NEURONS] = {0};
    din1_t f_state[NEURONS] = {0};
    din1_t c_state[NEURONS] = {0};
	din1_t o_state[NEURONS] = {0};

    int i, j=0;


    if(reset){
            for (i = 0; i < NEURONS; i++) {
				#pragma HLS PIPELINE
                h_state[i] = 0;
                x_state[i] = 0;
            }
        }

	// W * X

	for (i = 0; i < INPUT_DIM; i++) {
		for (j = 0; j < NEURONS; j++) {
		#pragma HLS PIPELINE
		//#pragma HLS unroll factor=2
		 i_state[j] += ((din1_t)kernel[i][j + NEURONS * 0] * (din1_t)input_data[i]);
		 f_state[j] += ((din1_t)kernel[i][j + NEURONS * 1] * (din1_t)input_data[i]);
		 c_state[j] += ((din1_t)kernel[i][j + NEURONS * 2] * (din1_t)input_data[i]);
		 o_state[j] += ((din1_t)kernel[i][j + NEURONS * 3] * (din1_t)input_data[i]);
		}
	}

    //W * h(t-1) + W * X
    for (i = 0; i < 100; i++) {
        for (j = 0; j < NEURONS; j++) {
		#pragma HLS PIPELINE
		//#pragma HLS unroll factor=2
        	i_state[j] += ((din1_t)recurrent_kernel[i][j + NEURONS * 0] * h_state[i]);
            f_state[j] += ((din1_t)recurrent_kernel[i][j + NEURONS * 1] * h_state[i]);
        	c_state[j] += ((din1_t)recurrent_kernel[i][j + NEURONS * 2] * h_state[i]);
        	o_state[j] += ((din1_t)recurrent_kernel[i][j + NEURONS * 3] * h_state[i]);
    	}
    }

    // W * h(t-1) + W * X + b
    for (i = 0; i < NEURONS ; ++i) {
	#pragma HLS PIPELINE
	//#pragma HLS unroll factor=2
    	i_state[i] += (din1_t)bias[i + NEURONS * 0];
    	i_state[i] =  (din1_t)sigmoid(i_state[i]);

    	f_state[i] += (din1_t)bias[i + NEURONS * 1];
    	f_state[i] =  (din1_t)sigmoid(f_state[i]);

    	c_state[i] += (din1_t)bias[i + NEURONS * 2];
    	c_state[i] =  (din1_t)tanh((float)c_state[i]);

    	o_state[i] += (din1_t)bias[i + NEURONS * 3];
    	o_state[i] =  (din1_t)sigmoid(o_state[i]);
	}

    //h, c
    for (i = 0; i < NEURONS; ++i) {
    #pragma HLS PIPELINE
	//#pragma HLS unroll factor=2
        x_state[i] = x_state[i] * f_state[i] + c_state[i] * i_state[i];
        h_state[i] = (din1_t)tanh((float)x_state[i]) * o_state[i];
        lstm_out[i] =(float)h_state[i];
    }

    return;

}
