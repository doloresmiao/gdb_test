
/* This is a automatically generated test. Do not modify */
#include "config.h"

#define COMPILE_INLINE_STATIC

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "build.h"
#include "config.h"
#include "gsl/gsl_mode.h"
#include "gsl/gsl_sf_airy.h"

#include "source.h"
#ifdef CUDA_COMPILE
__global__
#endif
// modify parameter here
void compute(double var_1) {
  // modify function + parameter here
  double comp = gsl_sf_airy_Ai(var_1, GSL_PREC_DOUBLE);
  #ifdef TEXT_OUTPUT
  printf("%.17g\n", comp);
  #else
  printf("%#08llX\n", *(long long*)&comp);
  #endif
}

int main(int argc, char** argv) {
/* Program variables */

  long long tmp_1_i = strtoull(argv[1], NULL, 16); double tmp_1 = *(double*)&tmp_1_i;

  compute
  #ifdef CUDA_COMPILE
  <<<1,1>>>
  #endif
  // modify parameter here
  (tmp_1);
  #ifdef CUDA_COMPILE
  cudaDeviceSynchronize();
  #endif

  return 0;
}
