#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define SCALAR double
//#define SCALAR float

#ifndef M_PI // standardy C99 i C11 nie wymagają definiowania stałej M_PI
#define M_PI (3.14159265358979323846)
#endif

int main(int argc, char **argv) { 
  // program obliczania przybliżenia PI za pomocą wzoru Leibniza
  //   PI = 4 * ( 1 - 1/3 + 1/5 - 1/7 + 1/9 - 1/11 + itd. )
  
  int rank, size;
  int max_liczba_wyrazow = 0;
  SCALAR local_sum_plus = 0.0, local_sum_minus = 0.0, global_sum_plus = 0.0, global_sum_minus = 0.0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    printf("Podaj maksymalną liczbę wyrazów do obliczenia przybliżenia PI\n");
    scanf("%d", &max_liczba_wyrazow);
  }

  // Rozsyłanie liczby iteracji do wszystkich procesów
  MPI_Bcast(&max_liczba_wyrazow, 1, MPI_INT, 0, MPI_COMM_WORLD);

  // Obliczanie zakresów iteracji dla każdego procesu
  int my_start = rank * (max_liczba_wyrazow / size);
  int my_end = (rank == size - 1) ? max_liczba_wyrazow : my_start + (max_liczba_wyrazow / size);

  for (int i = my_start; i < my_end; i++) {
    int j = 1 + 4 * i;
    local_sum_plus += 1.0 / j;
    local_sum_minus += 1.0 / (j + 2.0);
  }

  // Redukcja wyników lokalnych do procesu 0
  MPI_Reduce(&local_sum_plus, &global_sum_plus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_sum_minus, &global_sum_minus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    SCALAR pi_approx = 4 * (global_sum_plus - global_sum_minus);
    printf("PI obliczone: \t\t\t%20.15lf\n", pi_approx);
    printf("PI z biblioteki matematycznej: \t%20.15lf\n", M_PI);
  }

  MPI_Finalize();
  return 0;
}

