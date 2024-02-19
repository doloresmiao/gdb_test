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

ifndef $(NEW_CLANG)
NEW_CLANG = 0
endif

#ifeq ($(NEW_CLANG), 1)
OPTFLAGS = -enable-new-pm=0
#else
#OPTFLAGS = 
#endif

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

compile_all:
	$(MAKE) clean
	$(MAKE) CC=$(CC) OPT_LEVEL=3 FASTMATH=1
	mv ./test ./test_o3
	$(MAKE) clean
	$(MAKE) CC=$(CC) OPT_LEVEL=0 FASTMATH=0
	mv ./test ./test_o0

run:
	python3 ../../driver/auto_gdb.py -p ./test -a 0xc30c5912e502289f

clean:
	rm -rf *.o *.bc test *.out
	rm -rf workspace 0*/ output.map

main_bc:
	clang++ -emit-llvm minmax.c -c -o minmax.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm log.c -c -o log.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm error.c -c -o error.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm trig.c -c -o trig.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm airy.c -c -o airy.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)
	clang++ -emit-llvm test.c -c -o test.bc $(CUDAFLAGS) $(OPT) $(CFLAGS)

tracediff:
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < minmax.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < log.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < error.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < trig.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < airy.bc > /dev/null
	opt $(OPTFLAGS) -load ../../plugins/libTraceDiffPass.so -tracediff < test.bc > /dev/null

main: mk_workspace main_bc tracediff
	llc -filetype=obj test.bc
	llc -filetype=obj minmax.bc
	llc -filetype=obj log.bc
	llc -filetype=obj error.bc
	llc -filetype=obj trig.bc
	llc -filetype=obj airy.bc
	$(CC) test.o minmax.o log.o error.o trig.o airy.o -o test $(OPT) $(LDFLAGS)