#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define NEURONS 100
#define INPUT_DIM 2
#define TIME_STEP 50
#define TEST_X_SIZE TIME_STEP - 1
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
	float kernel[INPUT_DIM][NEURONS * GATES], //shape=(input_dim, self.units * 4)
	float recurrent_kernel[NEURONS][NEURONS * GATES], //shape=(self.units, self.units * 4)
	float bias[NEURONS * GATES],
    float lstm_out[NEURONS],
    int reset
    )
{


    static float h_state[NEURONS] = {0};
    static float C_state[NEURONS] = {0};
    float i_state[NEURONS] = {0};
    float f_state[NEURONS] = {0};
    float c_state[NEURONS] = {0};
	float o_state[NEURONS] = {0};

    int i, j;
    if(reset){
        for (i = 0; i < NEURONS; i++) {

            h_state[NEURONS] = 0;
            C_state[NEURONS] = 0;
        }
    }
    float temp_in, temp_k, temp_r, temp_b, temp_out;

	// W * X
	for (i = 0; i < INPUT_DIM; i++) {
		for (j = 0; j < NEURONS * GATES; j++) {
			temp_in = input_data[i];
			temp_k = kernel[i][j];
			if(j < NEURONS * 1) {
				i_state[j] += (temp_k * temp_in);
			}
			else if(j < NEURONS * 2) {
				f_state[j - NEURONS * 1] += (temp_k * temp_in);
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
        	temp_r = recurrent_kernel[i][j];
			if(j < NEURONS * 1) {
            	i_state[j] += (temp_r * h_state[i]);
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
    	temp_b = bias[i];
		if(i < NEURONS * 1) {
        	i_state[i] += temp_b;
			i_state[i] =  sigmoid(i_state[i]);
		}
		else if(i < NEURONS * 2) {
			f_state[i - NEURONS * 1] += temp_b;
	        f_state[i - NEURONS * 1] =  sigmoid(f_state[i - NEURONS * 1]);
		}
		else if(i < NEURONS * 3) {
			c_state[i - NEURONS * 2] += temp_b;
	        c_state[i - NEURONS * 2] =  tanh(c_state[i - NEURONS * 2]);
		}
		else if(i < NEURONS * 4) {
			o_state[i - NEURONS * 3] += temp_b;
	        o_state[i - NEURONS * 3] =  sigmoid(o_state[i - NEURONS * 3]);
	    }
	}

    //h, c
    for (i = 0; i < NEURONS; ++i) {
        C_state[i] = C_state[i] * f_state[i] + c_state[i] * i_state[i];
        h_state[i] = tanh(C_state[i]) * o_state[i];
    }

    for (i = 0; i < NEURONS; ++i) {
    	lstm_out[i] = h_state[i];
    }
    return;
}
