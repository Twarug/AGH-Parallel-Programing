#/bin/sh
set -x

export OMP_NUM_THREADS=3

gcc -fopenmp $1 -o out && ./out


# How to run:
#  ./run.sh openmp_petle_simple.c
#  ./run.sh openmp_petle.c