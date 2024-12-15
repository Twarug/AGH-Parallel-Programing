#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

typedef struct {
    char name[50];
    float pi;
    int num;
} Data;

#define BUFFER_SIZE 100

int main(int argc, char** argv) {
    int rank, size, tag, dest;
    Data data;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    tag = 0;
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    if (rank == 0) {
        Data data = {"Patryk", 21.37, 8080};
        void* buffer = malloc(BUFFER_SIZE);
        int position = 0;

        MPI_Pack(data.name, 50, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.pi, 1, MPI_FLOAT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.num, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);
        free(buffer);

        buffer = malloc(BUFFER_SIZE);
        Data recData;
        position = 0;

        MPI_Recv(buffer, BUFFER_SIZE, MPI_PACKED, prev, tag, MPI_COMM_WORLD, &status);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, recData.name, 50, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &recData.pi, 1, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &recData.num, 1, MPI_INT, MPI_COMM_WORLD);

        printf("Proces %d otrzymał dane: %s, %d, %.2f\n", rank, recData.name, recData.num, recData.pi);
        free(buffer);
        
    } else {
        void *buffer = malloc(BUFFER_SIZE);
        Data recData;
        int position = 0;

        MPI_Recv(buffer, BUFFER_SIZE, MPI_PACKED, prev, tag, MPI_COMM_WORLD, &status);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, recData.name, 50, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &recData.pi, 1, MPI_FLOAT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &recData.num, 1, MPI_INT, MPI_COMM_WORLD);

        printf("Proces %d otrzymał dane: %s, %d, %.2f\n", rank, recData.name, recData.num, recData.pi);
        free(buffer);

        recData.num += 1;

        position = 0;
        buffer = malloc(BUFFER_SIZE);
        MPI_Pack(recData.name, 50, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&recData.pi, 1, MPI_FLOAT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&recData.num, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);
        free(buffer);
    }

    MPI_Finalize();
    return 0;
}
