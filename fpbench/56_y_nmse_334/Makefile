ifndef $(CC)
CC = clang
endif

OPT = -O0

ifeq ($(OPT_LEVEL), 0)
OPT = -O0
endif

ifeq ($(OPT_LEVEL), 1)
OPT = -O1
endif

ifeq ($(OPT_LEVEL), 2)
OPT = -O2
endif

ifeq ($(OPT_LEVEL), 3)
OPT = -O3
endif

ifeq ($(FASTMATH), 1)
ifeq ($(CC), nvcc)
OPT += -use_fast_math
else
OPT += -ffast-math
endif
endif

ifndef $(LIBS)
ifeq ($(CC), clang++)
LIBS = -lcudart_static -ldl -lrt -lpthread
else ifeq ($(CC), nvcc)
LIBS = -lcudart_static -ldl -lrt -lpthread
else
LIBS = -lm
endif
endif

ifeq ($(CC), clang++)
CUDAFLAGS = -x cuda --cuda-gpu-arch=$(CUDA_CC_VERSION) -L/usr/local/cuda/lib64
else ifeq ($(CC), nvcc)
CUDAFLAGS = -arch=$(CUDA_CC_VERSION) --expt-relaxed-constexpr -x cu -DCUDA_COMPILE
endif

CFLAGS = 
LDFLAGS = $(LIBS) -Wl,-Map,output.map

default: main

mk_workspace:
	mkdir -p workspace



run:
	python3 ../../driver/auto_gdb.py -p ./test -a 0x433d75089307b26e

main: mk_workspace main_bc tracediff
	llc -filetype=obj test.bc
	$(CC) test.o -o test $(OPT) $(LDFLAGS)

clean:
	rm -rf *.o *.bc test *.out
	rm -rf workspace 0*/ output.map

main_bc:
	clang -O3 -emit-llvm test.cpp -g -c -o test.bc $(CUDAFLAGS) $(OPT) $(LDFLAGS)

tracediff:
	opt -enable-new-pm=0 -load ../../plugins/libTraceDiffPass.so -tracediff < test.bc > /dev/null