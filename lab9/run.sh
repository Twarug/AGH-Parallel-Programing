#/bin/sh
set -x

gcc -fopenmp $1 -o out && ./out
