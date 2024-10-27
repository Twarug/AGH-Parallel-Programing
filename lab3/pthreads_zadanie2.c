#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5

void* print_id(void* arg) {
  sleep(1);
  int thread_id = *((int*)arg);
  printf("Wątek systemowy ID: %lu, Identyfikator przekazany: %d\n", pthread_self(), thread_id);
  return NULL;
}

int main() {
    pthread_t threads[N];
    int thread_ids[N];

    for (int i = 0; i < N; i++) {
      #if 1
        thread_ids[i] = i;

        if (pthread_create(&threads[i], NULL, print_id, &thread_ids[i]) != 0) {
            fprintf(stderr, "Błąd przy tworzeniu wątku %d\n", i);
            exit(1);
        }
      #else
        if (pthread_create(&threads[i], NULL, print_id, &i) != 0) {
            fprintf(stderr, "Błąd przy tworzeniu wątku %d\n", i);
            exit(1);
        }
      #endif
    }

    for (int i = 0; i < N; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Błąd przy oczekiwaniu na zakończenie wątku %d\n", i);
        }
    }

    printf("Wszystkie wątki zakończyły działanie.\n");
    return 0;
}