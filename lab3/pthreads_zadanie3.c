#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int threadId;
    float value;
    char description[50];
} ThreadData;

ThreadData createThreadData(int threadId, float value, const char* description) {
    ThreadData data;

    data.threadId = threadId;
    data.value = value;
    strncpy(data.description, description, 50);

    return data;
}

// Funkcja wykonywana przez wątek
void* print_structure(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    printf("Wątek systemowy ID: %lu\n", pthread_self());
    printf("Identyfikator wątku: %d\n", data->threadId);
    printf("Wartość: %.2f\n", data->value);
    printf("Opis: %s\n\n", data->description);
    return NULL;
}

int main() {
    pthread_t threads[2];
    ThreadData threadData[2];

    threadData[0] = createThreadData(1, 10.55, "Wątek pierwszy");
    threadData[1] = createThreadData(2, 20.75, "Wątek drugi");

    // Tworzenie wątków
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&threads[i], NULL, print_structure, &threadData[i]) != 0) {
            fprintf(stderr, "Błąd przy tworzeniu wątku %d\n", i + 1);
            exit(1);
        }
    }

    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < 2; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Błąd przy oczekiwaniu na zakończenie wątku %d\n", i + 1);
        }
    }

    printf("Wszystkie wątki zakończyły działanie.\n");
    return 0;
}