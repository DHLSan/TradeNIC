#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NEURONS 100
#define INPUT_DIM 2
#define TIME_STEP 50
#define TEST_X_SIZE TIME_STEP-1
#define GATES 4 //input, forget, cell, output


float sigmoid(float x)
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

    static float h_state[NEURONS] = {0};
    static float x_state[NEURONS] = {0};
    float i_state[NEURONS] = {0};
    float f_state[NEURONS] = {0};
    float c_state[NEURONS] = {0};
	float o_state[NEURONS] = {0};

    int i, j=0;
    /*for (i = 0; i < NEURONS; ++i) {
    	//#pragma HLS PIPELINE II=1
    	//#pragma HLS unroll factor=2
            C_state[i] = C_state[i] * f_state[i] + c_state[i] * i_state[i];
            h_state[i] = tanh(C_state[i]) * o_state[i];
            lstm_out[i] = 21.20;
        }
*/

    if(reset){
            for (i = 0; i < NEURONS; i++) {
				//#pragma HLS PIPELINE II=1
                h_state[i] = 0;
                x_state[i] = 0;
            }
        }

	// W * X

	for (i = 0; i < INPUT_DIM; i++) {
		for (j = 0; j < NEURONS; j++) {
		//#pragma HLS PIPELINE II=1
		//#pragma HLS unroll factor=2
		 i_state[j] += (kernel[i][j + NEURONS * 0] * input_data[i]);
		 f_state[j] += (kernel[i][j + NEURONS * 1] * input_data[i]);
		 c_state[j] += (kernel[i][j + NEURONS * 2] * input_data[i]);
		 o_state[j] += (kernel[i][j + NEURONS * 3] * input_data[i]);
		}
	}

    //W * h(t-1) + W * X
    for (i = 0; i < 100; i++) {
        for (j = 0; j < NEURONS; j++) {
		//#pragma HLS PIPELINE II=1
		//#pragma HLS unroll factor=2
        	i_state[j] += (recurrent_kernel[i][j + NEURONS * 0] * h_state[i]);
            f_state[j] += (recurrent_kernel[i][j + NEURONS * 1] * h_state[i]);
        	c_state[j] += (recurrent_kernel[i][j + NEURONS * 2] * h_state[i]);
        	o_state[j] += (recurrent_kernel[i][j + NEURONS * 3] * h_state[i]);
    	}
    }

    // W * h(t-1) + W * X + b
    for (i = 0; i < NEURONS ; ++i) {
	//#pragma HLS PIPELINE II=1
	//#pragma HLS unroll factor=2
    	i_state[i] += bias[i + NEURONS * 0];
    	i_state[i] =  sigmoid(i_state[i]);

    	f_state[i] += bias[i + NEURONS * 1];
    	f_state[i] =  sigmoid(f_state[i]);

    	c_state[i] += bias[i + NEURONS * 2];
    	c_state[i] =  tanh(c_state[i]);

    	o_state[i] += bias[i + NEURONS * 3];
    	o_state[i] =  sigmoid(o_state[i]);
	}

    //h, c
    for (i = 0; i < NEURONS; ++i) {
	//#pragma HLS PIPELINE II=1
	//#pragma HLS unroll factor=2
        x_state[i] = x_state[i] * f_state[i] + c_state[i] * i_state[i];
        h_state[i] = tanh(x_state[i]) * o_state[i];
        lstm_out[i] = h_state[i];
    }

}
