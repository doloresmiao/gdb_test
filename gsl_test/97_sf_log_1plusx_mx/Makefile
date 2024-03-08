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

main: mk_workspace
	$(CC) -c ../err/error.c -o error.o $(CUDAFLAGS) $(OPT) $(CFLAGS)
	$(CC) -c ../specfunc/log.c -o log.o $(CUDAFLAGS) $(OPT) $(CFLAGS)
	$(CC) test.c error.o log.o -o test $(CUDAFLAGS) $(OPT) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o test *.out
	rm -rf workspace 0*/ output.map

func_analysis: mk_workspace
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c ../err/error.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c ../specfunc/log.c -emit-llvm

raise_precision:
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c ../err/error.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c ../specfunc/log.c -emit-llvm

print_results:
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c ../err/error.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c ../specfunc/log.c -emit-llvm
