#include <stdio.h>
#include <stdlib.h>
#include <ap_fixed.h>
#include <hls_math.h>

#define NEURONS 100
#define INPUT_DIM 2
#define TIME_STEP 50
#define TEST_X_SIZE TIME_STEP - 1
#define GATES 4 //input, forget, cell, output

typedef ap_fixed<32,4> din1_t;


/*din1_t sigmoid(din1_t x){
	return 1/(1+(exp((float)-x)));
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
	float kernel[INPUT_DIM][NEURONS * GATES], //shape=(input_dim, self.units * 4)
	float  recurrent_kernel[NEURONS][NEURONS * GATES], //shape=(self.units, self.units * 4)
	float  bias[NEURONS * GATES],
	float  lstm_out[NEURONS],
	int reset
    )
{

	#pragma HLS INTERFACE s_axilite port =input_data bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port =kernel bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port =recurrent_kernel bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port =bias bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port =lstm_out bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = reset bundle = BUS_A
	#pragma HLS INTERFACE s_axilite port = return bundle = BUS_A

    static din1_t h_state[NEURONS] = {0};
    static din1_t X_state[NEURONS] = {0};
    din1_t i_state[NEURONS] = {0};
    din1_t f_state[NEURONS] = {0};
    din1_t c_state[NEURONS] = {0};
	din1_t o_state[NEURONS] = {0};
	//#pragma HLS reset variable=h_state
	//#pragma HLS reset variable=X_state
    int i, j;

    din1_t temp_in, temp_k, temp_r, temp_b, temp_out;

    if(reset){
			for (i = 0; i < NEURONS; i++) {
				#pragma HLS PIPELINE
				h_state[i] = 0;
				X_state[i] = 0;
			}
		}

	// W * X

	for (i = 0; i < INPUT_DIM; i++) {
		temp_in = (din1_t)input_data[i];
		for (j = 0; j < NEURONS * GATES; j++) {
		#pragma HLS PIPELINE
		//#pragma HLS UNROLL factor=4
			temp_k = (din1_t)kernel[i][j];
			if(j < NEURONS * 1) {
				i_state[j] += (temp_k * temp_in);
			}
			else if(j < NEURONS * 2) {
				f_state[j - NEURONS * 1] += (temp_k* temp_in);
			}
			else if(j < NEURONS * 3) {
				c_state[j - NEURONS * 2] += (temp_k * temp_in);
			}
			else if(j < NEURONS * 4) {
				o_state[j - NEURONS * 3] += (temp_k * temp_in);
			}
		}
	}

    //W * h(t-1) + W * X
    for (i = 0; i < NEURONS; i++) {
        for (j = 0; j < NEURONS * GATES; j++) {
		#pragma HLS PIPELINE
		//#pragma HLS UNROLL factor=4
        	temp_r = (din1_t)recurrent_kernel[i][j];
			if(j < NEURONS * 1) {
            	i_state[j] += (temp_r* h_state[i]);
			}
			else if(j < NEURONS * 2) {
				f_state[j - NEURONS * 1] += (temp_r * h_state[i]);
			}
			else if(j < NEURONS * 3) {
				c_state[j - NEURONS * 2] += (temp_r * h_state[i]);
			}
			else if(j < NEURONS * 4) {
				o_state[j - NEURONS * 3] += (temp_r * h_state[i]);
			}
    	}
    }

    // W * h(t-1) + W * X + b
    for (i = 0; i < NEURONS * GATES; ++i) {
	#pragma HLS PIPELINE
   // #pragma HLS UNROLL factor=4
    	temp_b = (din1_t)bias[i];
		if(i < NEURONS * 1) {
        	i_state[i] += temp_b;
			i_state[i] =  (din1_t)sigmoid(i_state[i]);
		}
		else if(i < NEURONS * 2) {
			f_state[i - NEURONS * 1] += temp_b;
	        f_state[i - NEURONS * 1] =  (din1_t)sigmoid(f_state[i - NEURONS * 1]);
		}
		else if(i < NEURONS * 3) {
			c_state[i - NEURONS * 2] += temp_b;
	        c_state[i - NEURONS * 2] =  (din1_t)tanh(c_state[i-NEURONS*2]);
		}
		else if(i < NEURONS * 4) {
			o_state[i - NEURONS * 3] += temp_b;
	        o_state[i - NEURONS * 3] =  (din1_t)sigmoid(o_state[i - NEURONS * 3]);
	    }
	}

    //h, c
    for (i = 0; i < NEURONS; ++i) {
	#pragma HLS PIPELINE
	//#pragma HLS UNROLL factor=4

        X_state[i] = X_state[i] * f_state[i] + c_state[i] * i_state[i];
        h_state[i] = (din1_t)tanh(X_state[i])*o_state[i];
        lstm_out[i] = (float)h_state[i];
    }


    return;
}
