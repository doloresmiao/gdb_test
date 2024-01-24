ifndef $(CC)
CC = gcc
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

CFLAGS = -I/usr/local/include -I../ -I./ -g
LDFLAGS = $(LIBS) -Wl,-Map,output.map -g

ifeq ($(GLOBAL_TEXT_OUTPUT), 1)
CFLAGS += -DTEXT_OUTPUT
endif

default: main

mk_workspace:
	mkdir -p workspace
	mkdir -p workspace/func_analysis
	mkdir -p workspace/original_files

run:
	python3 ../driver/auto_gdb.py ./test 0xc30c5912e502289f

main: mk_workspace
	$(CC) $(CFLAGS) -c minmax.c -o minmax.o $(CUDAFLAGS) $(OPT) 
	$(CC) $(CFLAGS) -c trig.c -o trig.o $(CUDAFLAGS) $(OPT) 
	$(CC) $(CFLAGS) -c airy.c -o airy.o $(CUDAFLAGS) $(OPT)
	$(CC) $(CFLAGS) -c error.c -o error.o $(CUDAFLAGS) $(OPT)
	$(CC) $(CFLAGS) -c log.c -o log.o $(CUDAFLAGS) $(OPT)
	$(CC) $(CFLAGS) test.c minmax.o trig.o airy.o error.o log.o -o test $(CUDAFLAGS) $(OPT) $(LDFLAGS)

clean:
	rm -rf *.o test *.out
	rm -rf workspace 0*/ output.map

func_analysis: mk_workspace
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c minmax.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c trig.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c airy.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c error.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-function-analysis -c log.c -emit-llvm

raise_precision:
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c minmax.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c trig.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c airy.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c error.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-expand-precision -c log.c -emit-llvm

print_results:
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c minmax.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c trig.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c airy.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c error.c -emit-llvm
	clang++ $(CFLAGS) -Xclang -load -Xclang Ciel-plugin.so -Xclang -plugin -Xclang -pliner-gpu-print-results -c log.c -emit-llvm
