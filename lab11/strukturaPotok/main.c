#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mpi.h"

#define BUFFER_SIZE 100
#define A_COUNT_TO_DONE 5

typedef struct {
  char text[50];
  int a_count;
  int length;
  int is_done; 
} Data;

void count_a_in_text(Data *data) {
  data->a_count = 0;
  for (int i = 0; i < strlen(data->text); i++)
    if (tolower(data->text[i]) == 'a')
      data->a_count++;
}

void add_a(Data *data) {
  size_t len = strlen(data->text);
  if (len < 49) {  
    data->text[len] = 'a';
    data->text[len + 1] = '\0';
  }
}

void calc_len(Data *data) {
  data->length = strlen(data->text);
}

int main(int argc, char **argv) {
  int rank, size, tag = 0;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int next = (rank + 1) % size;
  int prev = (rank - 1 + size) % size;

  if (rank == 0) {
    Data dataset[3] = {
      {"Hello World"},
      {"Tesowanie testami"},
      {"Patryk Twardosz"}
    };
    int num_items = sizeof(dataset) / sizeof(Data);

    int done_count = 0;

    while (done_count < num_items) {
      for (int i = 0; i < num_items; i++) {
        if (dataset[i].is_done) continue;

        void *buffer = malloc(BUFFER_SIZE);
        int position = 0;

        MPI_Pack(dataset[i].text, 50, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&dataset[i].a_count, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&dataset[i].length, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
        MPI_Pack(&dataset[i].is_done, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

        MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);
        free(buffer);

        buffer = malloc(BUFFER_SIZE);
        position = 0;
        MPI_Recv(buffer, BUFFER_SIZE, MPI_PACKED, prev, tag, MPI_COMM_WORLD, &status);

        MPI_Unpack(buffer, BUFFER_SIZE, &position, dataset[i].text, 50, MPI_CHAR, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &dataset[i].a_count, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &dataset[i].length, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(buffer, BUFFER_SIZE, &position, &dataset[i].is_done, 1, MPI_INT, MPI_COMM_WORLD);
        free(buffer);

        if (dataset[i].a_count >= A_COUNT_TO_DONE) {
          dataset[i].is_done = 1;
          done_count++;
        }

        printf("Proces 0: \"%s\" - 'a': %d, Długość: %d, Zakończone: %d\n",
                dataset[i].text, dataset[i].a_count, dataset[i].length, dataset[i].is_done);
      }
    }

    Data termination_data = {"", 0, 0, 1};
    void *buffer = malloc(BUFFER_SIZE);
    int position = 0;
    MPI_Pack(termination_data.text, 50, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
    MPI_Pack(&termination_data.a_count, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
    MPI_Pack(&termination_data.length, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
    MPI_Pack(&termination_data.is_done, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

    for (int i = 1; i < size; i++) {
      MPI_Send(buffer, position, MPI_PACKED, i, tag, MPI_COMM_WORLD);
    }
    free(buffer);

  } else {
    while (1) {
      void *buffer = malloc(BUFFER_SIZE);
      int position = 0;

      MPI_Recv(buffer, BUFFER_SIZE, MPI_PACKED, prev, tag, MPI_COMM_WORLD, &status);

      Data data;
      MPI_Unpack(buffer, BUFFER_SIZE, &position, data.text, 50, MPI_CHAR, MPI_COMM_WORLD);
      MPI_Unpack(buffer, BUFFER_SIZE, &position, &data.a_count, 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, BUFFER_SIZE, &position, &data.length, 1, MPI_INT, MPI_COMM_WORLD);
      MPI_Unpack(buffer, BUFFER_SIZE, &position, &data.is_done, 1, MPI_INT, MPI_COMM_WORLD);
      free(buffer);

      if (data.is_done && strlen(data.text) == 0) { 
        if(rank == size - 1) {
          break;
        } else {
          void *buffer = malloc(BUFFER_SIZE);
          int position = 0;
          MPI_Pack(data.text, 50, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
          MPI_Pack(&data.a_count, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
          MPI_Pack(&data.length, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
          MPI_Pack(&data.is_done, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

          MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);

          break;
        }
      }

      if (rank == 1) {
        count_a_in_text(&data);
      } else if (rank == 2) {
        add_a(&data);
      } else if (rank == 3) {
        calc_len(&data);
      }

      buffer = malloc(BUFFER_SIZE);
      position = 0;

      MPI_Pack(data.text, 50, MPI_CHAR, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
      MPI_Pack(&data.a_count, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
      MPI_Pack(&data.length, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);
      MPI_Pack(&data.is_done, 1, MPI_INT, buffer, BUFFER_SIZE, &position, MPI_COMM_WORLD);

      MPI_Send(buffer, position, MPI_PACKED, next, tag, MPI_COMM_WORLD);
      free(buffer);
    }
  }

  printf("Proces %d zakończył pracę\n", rank);

  MPI_Finalize();
  return 0;
}
