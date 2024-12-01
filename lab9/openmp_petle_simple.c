#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 18

int main ()
{
  // ustawianie liczby wątków przez 
  // omp_set_dynamic(0);

  double a[WYMIAR];

#pragma omp  parallel for default(none) shared(a)
  for(int i=0;i<WYMIAR;i++) a[i]=1.02*i;

  // pętla sekwencyjna
  double suma=0.0;
  for(int i=0;i<WYMIAR;i++) {
      suma += a[i];
  }
  
  printf("Suma wyrazĂłw tablicy: %lf\n\n\n", suma);

  { // 4. bazujące na zmiennej środowiskoewej
  
    double suma_parallel=0.0;
#pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered
    for(int i=0;i<WYMIAR;i++) {
      int id_w = omp_get_thread_num();
        suma_parallel += a[i];
  #pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w); 
    }
    
    printf("\nSuma wyrazów tablicy równolegle (OMP_NUM_THREADS): %lf\n\n\n", suma_parallel);
  }
  
  { // 5.1. static, porcja 3
  
    double suma_parallel=0.0;
#pragma omp parallel for num_threads(4) schedule(static, 3) default(none) shared(a) reduction(+:suma_parallel) ordered
    for(int i=0;i<WYMIAR;i++) {
      int id_w = omp_get_thread_num();
        suma_parallel += a[i];
  #pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w); 
    }
    
    printf("\nSuma wyrazów tablicy równolegle (static, porcja 3): %lf\n\n\n", suma_parallel);
  }

  { // 5.2. static, porcja default
  
    double suma_parallel=0.0;
#pragma omp parallel for num_threads(4) schedule(static) default(none) shared(a) reduction(+:suma_parallel) ordered
    for(int i=0;i<WYMIAR;i++) {
      int id_w = omp_get_thread_num();
        suma_parallel += a[i];
  #pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w); 
    }
    
    printf("\nSuma wyrazów tablicy równolegle (static, porcja default): %lf\n\n\n", suma_parallel);
  }

  { // 5.3. dynamic, porcja 2
  
    double suma_parallel=0.0;
#pragma omp parallel for num_threads(4) schedule(dynamic, 2) default(none) shared(a) reduction(+:suma_parallel) ordered
    for(int i=0;i<WYMIAR;i++) {
      int id_w = omp_get_thread_num();
        suma_parallel += a[i];
  #pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w); 
    }
    
    printf("\nSuma wyrazów tablicy równolegle (dynamic, porcja 2): %lf\n\n\n", suma_parallel);
  }

  { // 5.4. dynamic, porcja default
  
    double suma_parallel=0.0;
#pragma omp parallel for num_threads(4) schedule(dynamic) default(none) shared(a) reduction(+:suma_parallel) ordered
    for(int i=0;i<WYMIAR;i++) {
      int id_w = omp_get_thread_num();
        suma_parallel += a[i];
  #pragma omp ordered
        printf("a[%2d]->W_%1d  \n",i,id_w); 
    }
    
    printf("\nSuma wyrazów tablicy równolegle (dynamic, porcja default): %lf\n\n\n", suma_parallel);
  }
}