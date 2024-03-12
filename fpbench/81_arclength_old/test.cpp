
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
void compute(double var_1) {//, double var_2, double var_3) {
  // modify function + parameter here
  double comp = arclength_old(var_1);//, var_2, var_3);
  printf("%#08llX\n", *(long long*)&comp);
}

int main(int argc, char** argv) {
/* Program variables */

  long long tmp_1_i = strtoull(argv[1], NULL, 16); double tmp_1 = *(double*)&tmp_1_i;
  //long long tmp_2_i = strtoull(argv[2], NULL, 16); double tmp_2 = *(double*)&tmp_2_i;
  //long long tmp_3_i = strtoull(argv[3], NULL, 16); double tmp_3 = *(double*)&tmp_3_i;

  compute
  #ifdef CUDA_COMPILE
  <<<1,1>>>
  #endif
  // modify parameter here
  (tmp_1);//, tmp_2, tmp_3);
  #ifdef CUDA_COMPILE
  cudaDeviceSynchronize();
  #endif

  return 0;
}
