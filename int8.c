#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "arm_neon.h"

#define NUM_RANDOM 524288
// pow(2, 19)
// NUM_RANDOM * 4 MACs
#define NUM_REPEAT 256

// INT8 quantization inner product using ARMv7-A NEON simulation
int main() {
    int * weight = (int *)malloc(sizeof(int) * NUM_RANDOM);
    int * inputs = (int *)malloc(sizeof(int) * NUM_RANDOM);
    for (int i = 0; i < NUM_RANDOM; ++i) {
        weight[i] = rand();
	inputs[i] = rand();
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int accum_data[] = {0, 0, 0, 0};
    int32x4_t accum = vld1q_s32(accum_data);
    for (int r = 0; r < NUM_REPEAT; ++r) {
        for (int i = 0; i < NUM_RANDOM; i += 2) {
            // pack weight and inputs
            char weight_[8];
            char inputs_[8];
	    for (int j = 0; j < 2; ++j) {
                int we = weight[i + j];
                int in = inputs[i + j];
		for (int k = 0; k < 4; ++k) {
                    weight_[j * 4 + k] = (char)(we & 0xFF);
		    inputs_[j * 4 + k] = (char)(in & 0xFF);
		    we >>= 8;
		    in >>= 8;
                }
            }

	    // inner product + accum
            accum = vaddq_s32(accum, vpaddlq_s16(vmull_s8(
                              vld1_s8(weight_), vld1_s8(inputs_))));
        }
    }

    gettimeofday(&end, NULL);
    printf("spent %lf seconds\n", end.tv_sec - start.tv_sec +
                         (double)(end.tv_usec - start.tv_usec) / 1000000);
    free(weight);
    free(inputs);
    return 0;
}

