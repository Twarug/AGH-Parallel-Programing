#!/bin/sh

export OMP_NUM_THREADS=16

gcc -O3 -fopenmp calka_omp.c -o out && ./out