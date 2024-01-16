#!/bin/sh
#
# Quick benchmarking script.  Not elegant, but good enough to answer
# the exam questions.  We don't expect that the students automate this
# - it's fine to run things by hand.

set -e
THREADS="1 2 4 8"

make

# Produce data files.
mkdir -p datafiles

mkvector() {
    f=datafiles/${1}.vdns
    if ! [ -f $f ]; then
        echo "Generating $f..."
        ./random-vector $1 $f
    fi
}

mkdense() {
    f=datafiles/${1}_${2}.mdns
    if ! [ -f $f ]; then
        echo "Generating $f..."
        ./random-matrix-dense $1 $2 $f
    fi
}

mkcsr() {
    n=$1
    m=$2
    f=datafiles/${n}_${m}.mcsr
    if ! [ -f $f ]; then
        echo "Generating $f..."
        ./random-matrix-csr $n $m 0 0.1 $f
    fi
}

getseconds() {
    cut -d ' ' -f 3
}

calc() {
    echo "scale=10; $*" | bc
}

RUNS=10

mul_vv() {
    OMP_NUM_THREADS=${1} ./benchmark-mul-vv datafiles/${2}.vdns datafiles/${2}.vdns $RUNS | getseconds
}

bench_mul_vv(){
    echo
    echo "mul-vv weak scaling"

    k=$((8*1024*1024))
    mkvector $k
    d=$(mul_vv 1 $k)
    base=$(calc "$k / $d")
    for t in $THREADS; do
        n=$(($t * $k))
        mkvector $n
        printf "%.2d: " $t
        d=$(mul_vv $t $n)
        calc "($n / $d) / $base"
    done

    echo
    echo "mul-vv strong scaling"

    n=$k
    mkvector $n
    base=$(mul_vv 1 $n)
    for t in $THREADS; do
        printf "%.2d: " $t
        d=$(mul_vv $t $n)
        calc "$base/$d"
    done
}

mul_mv() {
    OMP_NUM_THREADS=${1} ./benchmark-mul-mv datafiles/${2}_${3}.mdns datafiles/${3}.vdns $RUNS | getseconds
}

bench_mul_mv() {
    echo
    echo "mul-mv weak scaling"

    n=$((8*1024))
    k=$((1024))
    mkdense $n $k
    mkvector $k
    d=$(mul_mv 1 $n $k)
    base=$(calc "($n*$k) / $d")
    for t in $THREADS; do
        m=$(($t * $k))
        mkdense $n $m
        mkvector $m
        printf "%.2d: " $t
        d=$(mul_mv $t $n $m)
        calc "(($n*$m) / $d) / $base"
    done

    echo
    echo "mul-mv strong scaling"
    m=$k
    mkdense $n $m
    mkvector $m
    base=$(mul_mv 1 $n $m)
    for t in $THREADS; do
        printf "%.2d: " $t
        d=$(mul_mv $t $n $m)
        calc "$base/$d"
    done
}

mul_mTv() {
    OMP_NUM_THREADS=${1} ./benchmark-mul-mTv datafiles/${2}_${3}.mdns datafiles/${2}.vdns $RUNS | getseconds
}

bench_mul_mTv() {
    echo
    echo "mul-mTv weak scaling"

    n=$((8*1024))
    k=$((1024))
    mkdense $n $k
    mkvector $n
    d=$(mul_mTv 1 $n $k)
    base=$(calc "($n*$k) / $d")
    for t in $THREADS; do
        m=$(($t * $k))
        mkdense $n $m
        mkvector $n
        printf "%.2d: " $t
        d=$(mul_mTv $t $n $m)
        calc "(($n*$m) / $d) / $base"
    done

    echo
    echo "mul-mTv strong scaling"
    m=$k
    mkdense $n $m
    mkvector $n
    base=$(mul_mTv 1 $n $m)
    for t in $THREADS; do
        printf "%.2d: " $t
        d=$(mul_mTv $t $n $m)
        calc "$base/$d"
    done
}

mul_spmv() {
    OMP_NUM_THREADS=${1} ./benchmark-mul-spmv datafiles/${2}_${3}.mcsr datafiles/${3}.vdns $RUNS | getseconds
}

bench_mul_spmv() {
    echo
    echo "mul-spmv weak scaling"

    n=$((32*1024))
    k=$((4*1024))
    mkcsr $n $k
    mkvector $k
    d=$(mul_spmv 1 $n $k)
    base=$(calc "($n*$k) / $d")
    for t in $THREADS; do
        m=$(($t * $k))
        mkcsr $n $m
        mkvector $m
        printf "%.2d: " $t
        d=$(mul_spmv $t $n $m)
        calc "(($n*$m) / $d) / $base"
    done

    echo
    echo "mul-spmv strong scaling"
    m=$k
    mkcsr $n $m
    mkvector $m
    base=$(mul_spmv 1 $n $m)
    for t in $THREADS; do
        printf "%.2d: " $t
        d=$(mul_spmv $t $n $m)
        calc "$base/$d"
    done
}

mul_spmTv() {
    OMP_NUM_THREADS=${1} ./benchmark-mul-spmTv datafiles/${2}_${3}.mcsr datafiles/${2}.vdns $RUNS | getseconds
}

bench_mul_spmTv() {
    echo
    echo "mul-spmTv weak scaling"

    n=$((32*1024))
    k=$((4*1024))
    mkcsr $n $k
    mkvector $n
    d=$(mul_spmTv 1 $n $k)
    base=$(calc "($n*$k) / $d")
    for t in $THREADS; do
        m=$(($t * $k))
        mkcsr $n $m
        mkvector $n
        printf "%.2d: " $t
        d=$(mul_spmTv $t $n $m)
        calc "(($n*$m) / $d) / $base"
    done

    echo
    echo "mul-spmTv strong scaling"
    m=$k
    mkcsr $n $m
    mkvector $n
    base=$(mul_spmTv 1 $n $m)
    for t in $THREADS; do
        printf "%.2d: " $t
        d=$(mul_spmTv $t $n $m)
        calc "$base/$d"
    done
}

bench_mul_vv
bench_mul_mv
bench_mul_mTv
bench_mul_spmv
bench_mul_spmTv
