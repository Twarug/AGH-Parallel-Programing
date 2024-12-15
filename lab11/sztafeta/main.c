#include <stdlib.h>
#include <stdio.h>

#include "mpi.h"

int main(int argc, char** argv) {
  int rank, size, next, prev, value;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  prev = (rank - 1 + size) % size;
  next = (rank + 1) % size;
  
#if 0

  if (rank == 0) {
    value = 1;
    printf("Proces %d inicjuje sztafetę z liczbą %d\n", rank, value);

    MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);

    printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, value, status.MPI_SOURCE);
  } else {
    MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
    
    printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, value, status.MPI_SOURCE);
    value++;

    MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
  }

#else
  
  if (rank == 0) {
    value = 1;
    printf("Proces %d inicjuje sztafetę z liczbą %d\n", rank, value);

    MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
  }
  else {
    MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, &status);
    
    printf("Proces %d odebrał liczbę %d od procesu %d\n", rank, value, status.MPI_SOURCE);
    value++;

    if (rank != size - 1)
      MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
  }

#endif

  MPI_Finalize();
  return 0;
}