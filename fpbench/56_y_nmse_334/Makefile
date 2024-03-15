ifndef $(CC)
CC = clang
endif

ifndef (LLVM_PASS)
LLVM_PASS = 0
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
else
CUDAFLAGS =
endif

CFLAGS = 
LDFLAGS = $(LIBS) -Wl,-Map,output.map -no-pie

default: main

mk_workspace:
	mkdir -p workspace

compile_all:
	rm -rf *.o *.bc *.out
	rm -rf workspace 0*/ output.map
	$(MAKE) CC=$(CC) OPT_LEVEL=3 FASTMATH=1
	mv ./test ./test_o3
	rm -rf *.o *.bc *.out
	rm -rf workspace 0*/ output.map
	$(MAKE) CC=$(CC) OPT_LEVEL=0 FASTMATH=0
	mv ./test ./test_o0

run:
	python3 ../../driver/binary_trace_comp.py -p ./test -a 0x433d75089307b26e

main: mk_workspace main_bc $(if $(findstring $(LLVM_PASS),1),tracediff)
	llc -filetype=obj test.bc
	$(CC) test.o -o test $(OPT) $(LDFLAGS)

clean:
	rm -rf *.o *.bc test_* test *.out
	rm -rf workspace 0*/ output.map

main_bc:
	clang -O3 -emit-llvm test.cpp -g -c -o test.bc $(CUDAFLAGS) $(OPT)

tracediff:
	opt -enable-new-pm=0 -load ../../plugins/libTraceDiffPass.so -tracediff < test.bc > /dev/null