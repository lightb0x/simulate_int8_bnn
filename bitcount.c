#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "arm_neon.h"

#define NUM_RANDOM 524288
// pow(2, 19)
// NUM_RANDOM * 32 MACs
#define NUM_REPEAT 256

// XNOR -> bitcount using ARMv7-A NEON intrinsics
int main() {
    int * weight = (int *)malloc(sizeof(int) * NUM_RANDOM);
    int * inputs = (int *)malloc(sizeof(int) * NUM_RANDOM);
    int * tmp = (int *)malloc(sizeof(int) * 4);
    if (weight == NULL || inputs == NULL || tmp == NULL) {
        printf("not enough memory\n");
        return -1;
    }
    for (int i = 0; i < NUM_RANDOM; ++i) {
        weight[i] = rand();
	inputs[i] = rand();
    } // weight, inputs frozen now
    struct timeval start, end;
    gettimeofday(&start, NULL);

    int zeros[] = {0, 0, 0, 0};
    int32x4_t accum = vld1q_s32(zeros);
    for (int r = 0; r < NUM_REPEAT; ++r) {
        for (int i = 0; i < NUM_RANDOM; i += 4) {
            // xnor
            for (int j = 0; j < 4; ++j) {
                tmp[j] = ~(inputs[i + j] ^ weight[i + j]);
            }
	    // pack
            char for_load[32];
            for (int j = 0; j < 4; ++j) {
                int tmp_s8 = tmp[j];
                for (int k = 0 ; k < 4; ++k) {
                    for_load[j * 4 + k] = (char)(tmp_s8 & 0xFF);
		    tmp_s8 >>= 8;
                }
            }
            // bitcount & accum
            accum = vaddq_s32(accum,
                vpaddlq_s16(vpaddlq_s8(vcntq_s8(vld1q_s8(for_load)))));
        }
    }
    gettimeofday(&end, NULL);
    printf("spent %lf seconds\n", end.tv_sec - start.tv_sec +
                         (double)(end.tv_usec - start.tv_usec) / 1000000);
    free(weight);
    free(inputs);
    return 0;
}

