#!/bin/sh


gcc -O3 -fopenmp calka_omp.c -o out

export OMP_NUM_THREADS=16
./out