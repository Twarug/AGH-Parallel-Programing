#include<stdlib.h>
#include<stdio.h>
#include<omp.h>
#include<math.h>

#define N 1000000

int main(){

  double* A = malloc((N+2)*sizeof(double));
  double* B = malloc((N+2)*sizeof(double));
  double suma;

  for(int i=0;i<N+2;i++) A[i] = (double)i/N;
  for(int i=0;i<N+2;i++) B[i] = 1.0 - (double)i/N;

  double t1 = omp_get_wtime();
  for(int i=0; i<N; i++){
    A[i] += A[i+2] + sin(B[i]);
  }
  t1 = omp_get_wtime() - t1;

  suma = 0.0;
  for(int i=0;i<N+2;i++) suma+=A[i];
  printf("suma %lf, czas obliczen %lf\n", suma, t1);



  for(int i=0;i<N+2;i++) A[i] = (double)i/N;
  for(int i=0;i<N+2;i++) B[i] = 1.0 - (double)i/N;

  double temp[N + 2];

  t1 = omp_get_wtime();

  // wersja rĂłwnolegĹa
  #pragma omp parallel for shared(A, B, temp) num_threads(2)
  for(int i = 0; i < N; i++)
    temp[i] = A[i] + A[i+2] + sin(B[i]);

  #pragma omp parallel for shared(A, temp) num_threads(2)
  for(int i = 0; i < N; i++)
    A[i] = temp[i];

  t1 = omp_get_wtime() - t1;

  suma = 0.0;
  for(int i=0;i<N+2;i++) suma+=A[i];
  printf("suma %lf, czas obliczen rownoleglych %lf\n", suma, t1);
}