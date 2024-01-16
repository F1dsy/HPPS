CC?=gcc
# Good CFLAGS for maximum performance - use when benchmarking
CFLAGS?=-std=c99 -Wall -Wextra -pedantic -pthread -fopenmp -O3 -lm
# Good CFLAGS when debugging - but slow down execution!
#CFLAGS?=-std=c99 -Wall -Wextra -pedantic -pthread -fopenmp -g -fsanitize=address -fsanitize=undefined -lm
PROGRAMS=print-type \
         random-scalar print-scalar \
	 random-vector print-vector mul-vv \
         random-matrix-dense print-matrix-dense mul-mv mul-mTv \
	 random-matrix-csr print-matrix-csr mul-spmv mul-spmTv \
	 benchmark-mul-vv benchmark-mul-mv benchmark-mul-spmv benchmark-mul-mTv benchmark-mul-spmTv \
	 copy-vector copy-matrix-dense copy-matrix-csr \
	 cmp-scalar cmp-vector

# Disable builtin rules (don't worry about the next two lines).
.SUFFIXES:
.SECONDARY:

all: $(PROGRAMS)

%: %.o matlib.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) $^ $(CFLAGS) -c

datafiles: random-vector random-matrix-dense random-matrix-csr
	mkdir -p datafiles
	./random-vector               100     datafiles/100.vdns
	./random-matrix-dense 1000000 100     datafiles/1000000_100.mdns
	./random-matrix-csr   1000000 100 1 1 datafiles/1000000_100_1_1.mcsr
	./random-matrix-csr   1000000 100 0 1 datafiles/1000000_100_0_1.mcsr

	./random-vector           1000000     datafiles/1000000.vdns
	./random-matrix-dense 100 1000000     datafiles/100_1000000.mdns
	./random-matrix-csr   100 1000000 1 1 datafiles/100_1000000_1_1.mcsr
	./random-matrix-csr   100 1000000 0 1 datafiles/100_1000000_0_1.mcsr

	./random-vector          10000000     datafiles/10000000.vdns
	./random-matrix-dense 10 10000000     datafiles/10_10000000.mdns
	./random-matrix-csr   10 10000000 1 1 datafiles/10_10000000_1_1.mcsr
	./random-matrix-csr   10 10000000 0 1 datafiles/10_10000000_0_1.mcsr

	./random-matrix-dense 100 1000000     datafiles/100_1000000.mdns
	./random-matrix-csr   100 1000000 1 1 datafiles/100_1000000_1_1.mcsr
	./random-matrix-csr   100 1000000 0 1 datafiles/100_1000000_0_1.mcsr

benchmark: benchmark-mul-mv benchmark-mul-spmv
	./benchmark-mul-mv   datafiles/1000000_100.mdns     datafiles/100.vdns 100
	./benchmark-mul-spmv datafiles/1000000_100_1_1.mcsr datafiles/100.vdns 100
	./benchmark-mul-spmv datafiles/1000000_100_0_1.mcsr datafiles/100.vdns 100

	./benchmark-mul-mv   datafiles/100_1000000.mdns     datafiles/1000000.vdns 100
	./benchmark-mul-spmv datafiles/100_1000000_1_1.mcsr datafiles/1000000.vdns 100
	./benchmark-mul-spmv datafiles/100_1000000_0_1.mcsr datafiles/1000000.vdns 100

	./benchmark-mul-mv   datafiles/10_10000000.mdns     datafiles/10000000.vdns 100
	./benchmark-mul-spmv datafiles/10_10000000_1_1.mcsr datafiles/10000000.vdns 100
	./benchmark-mul-spmv datafiles/10_10000000_0_1.mcsr datafiles/10000000.vdns 100

	./benchmark-mul-mTv   datafiles/100_1000000.mdns     datafiles/100.vdns 100
	./benchmark-mul-spmTv datafiles/100_1000000_1_1.mcsr datafiles/100.vdns 100
	./benchmark-mul-spmTv datafiles/100_1000000_0_1.mcsr datafiles/100.vdns 100

clean:
	rm -rf *.o $(PROGRAMS) datafiles

