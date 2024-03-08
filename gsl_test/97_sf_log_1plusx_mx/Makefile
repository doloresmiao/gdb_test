ifndef $(CC)
CC = clang
endif

ifndef $(OPT_LEVEL)
OPT_LEVEL = 0
endif

ifndef $(FASTMATH)
FASTMATH = 0
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
ifeq ($(CC), nvcc)
LIBS += -lcudart_static -ldl -lrt -lpthread
else
LIBS +=  -static -lm
endif
endif

ifeq ($(CC), nvcc)
CUDAFLAGS = -arch=$(CUDA_CC_VERSION) --expt-relaxed-constexpr -x cu -DCUDA_COMPILE
endif

CFLAGS = -I/usr/local/include -I../ -I./
LDFLAGS = $(LIBS) -Wl,-Map,output.map

ifeq ($(GLOBAL_TEXT_OUTPUT), 1)
CFLAGS += -DTEXT_OUTPUT
endif

default: main

mk_workspace:
	mkdir -p workspace
	mkdir -p workspace/func_analysis
	mkdir -p workspace/original_files

run:
	cat input.txt | ./test `xargs`

main_bc:
	clang++ -emit-llvm log.c -c -o log.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm error.c -c -o error.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm test.c -c -o test.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)

tracediff:
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < log.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < error.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < test.bc > /dev/null

main: mk_workspace main_bc $(if $(findstring $(LLVM_PASS),1),tracediff)
	llc -filetype=obj test.bc
	llc -filetype=obj log.bc
	llc -filetype=obj error.bc
	$(CC) test.o log.o error.o -o test $(OPT) $(LDFLAGS)

clean:
	rm -rf *.o test *.out *.bc
	rm -rf workspace 0*/ output.map
