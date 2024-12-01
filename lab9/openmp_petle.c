#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 10

int main ()
{
  double a[WYMIAR][WYMIAR];

  for(int i=0;i<WYMIAR;i++) for(int j=0;j<WYMIAR;j++) a[i][j]=1.02*i+1.01*j;

  // podwójna pętla - sekwencyjnie
  double suma=0.0;
  for(int i=0;i<WYMIAR;i++) {
    for(int j=0;j<WYMIAR;j++) {
      suma += a[i][j];
    }
  }
  
  printf("Suma wyrazĂłw tablicy: %lf\n\n", suma);

  omp_set_nested(1);


  { // 7.1. dekompozycja wierszowa
    double sum_parallel=0.0;
    #pragma omp parallel for schedule(static, 2) reduction(+:sum_parallel) default(none) shared(a) ordered
    for(int i = 0; i < WYMIAR; i++) {
      #pragma omp ordered
      {
        for(int j = 0; j < WYMIAR; j++) {
          sum_parallel += a[i][j];
          printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num()); 
        }
        printf("\n");
      }
    }

    printf("Suma wyrazów tablicy równolegle: %lf\n\n", sum_parallel);
  }
  
  { // 7.2. dekompozycja kolumnowa wew (3.0)
    double sum_parallel=0.0;
    for(int i = 0; i < WYMIAR; i++) {
      #pragma omp parallel for schedule(dynamic) reduction(+:sum_parallel) default(none) shared(a, i) ordered
      for(int j = 0; j < WYMIAR; j++) {
        sum_parallel += a[i][j];
        #pragma omp ordered
        printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num());
      }
      printf("\n");
    }

    printf("Suma wyrazów tablicy równolegle: %lf\n\n", sum_parallel);
  }
  
  { // 7.3. dekompozycja kolumnowa zew (3.5)
    double sum_parallel=0.0;
  
    #pragma omp parallel default(none) shared(a, sum_parallel)
    {
      double local_sum = 0;

      #pragma omp for schedule(static) ordered
      for(int j = 0; j < WYMIAR; j++) {
        for(int i = 0; i < WYMIAR; i++) {
          local_sum += a[i][j];
          
          #pragma omp ordered
          printf("(%1d,%1d)-W_%1d ", i, j, omp_get_thread_num()); 
        }
        #pragma omp ordered
        printf("\n");
      }

      #pragma omp critical
      {
        sum_parallel += local_sum;
      }
    }

    printf("Suma wyrazów tablicy równolegle: %lf\n\n", sum_parallel);
  }
  
  omp_set_nested(1); // Włączanie zagnieżdżonych obszarów równoległych

  { // 7.4. dekompozycja 2D
    double sum_parallel=0.0;

#pragma omp parallel for schedule(static, 2) num_threads(3) default(none) shared(a) reduction(+:sum_parallel) ordered
    for(int i = 0; i < WYMIAR; i++) {
      int id_w = omp_get_thread_num();
  #pragma omp parallel for schedule(static, 2) num_threads(2) firstprivate(id_w) reduction(+:sum_parallel) ordered      
      for(int j = 0; j < WYMIAR; j++) {
        sum_parallel += a[i][j];
        
        #pragma omp ordered
        printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w,omp_get_thread_num()); 
        if (j == WYMIAR - 1)
          printf("\n");  
      }
    }

    printf("Suma wyrazów tablicy równolegle: %lf\n\n", sum_parallel);
  }

  { // 7.5. dekompozycja 2D bez rozmiaru porcji (4.5)
    double sum_parallel=0.0;

#pragma omp parallel for schedule(static) default(none) shared(a) reduction(+:sum_parallel) ordered
    for(int i = 0; i < WYMIAR; i++) {
      int id_w = omp_get_thread_num();
  #pragma omp parallel for schedule(static) firstprivate(id_w) reduction(+:sum_parallel) ordered
      for(int j = 0; j < WYMIAR; j++) {
        sum_parallel += a[i][j];

        #pragma omp ordered
        {
          printf("(%1d,%1d)-W_%1d,%1d ",i,j,id_w, omp_get_thread_num());
          if (j == WYMIAR - 1)
            printf("\n");  
        }
      }
    }

    printf("Suma wyrazów tablicy równolegle: %lf\n\n", sum_parallel);
  }

  { // 8. dekompozycja wierszowa - zrównoleglenie pętli wewnętrznej (5.0)
    double total_sum = 0.0;
    double sums_parallel[WYMIAR]; 

    #pragma omp parallel for schedule(static, 1) default(none) shared(a, sums_parallel)
    for(int j = 0; j < WYMIAR; j++) {
      #pragma omp parallel for schedule(static, 1) default(none) shared(a, j, sums_parallel)
      for(int i = 0; i < WYMIAR; i++)
        sums_parallel[i] += a[i][j];
    }

    for (int i = 0; i < WYMIAR; i++)
      total_sum += sums_parallel[i];

    printf("Suma wyrazów tablicy równolegle: %lf\n\n", total_sum);
  }

}