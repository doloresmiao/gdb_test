
/* This is a automatically generated test. Do not modify */
#ifdef __CUDACC__
#define CUDA_COMPILE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "common.h"
#include "benchmarks.h"

#ifdef CUDA_COMPILE
__global__
#endif
// modify parameter here
void compute(float var_1, float var_2) {
  // modify function + parameter here
  float comp = hypot32(var_1, var_2);
  printf("%#08X\n", *(unsigned int*)&comp);
}

int main(int argc, char** argv) {
/* Program variables */

  int tmp_1_i = strtoul(argv[1], NULL, 16); float tmp_1 = *(float*)&tmp_1_i;
  int tmp_2_i = strtoul(argv[2], NULL, 16); float tmp_2 = *(float*)&tmp_2_i;

  compute
  #ifdef CUDA_COMPILE
  <<<1,1>>>
  #endif
  // modify parameter here
  (tmp_1, tmp_2);
  #ifdef CUDA_COMPILE
  cudaDeviceSynchronize();
  #endif

  return 0;
}
