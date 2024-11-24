#include <pthread.h>
#include <stdio.h>

static int threads_to_wait;   // Liczba wątków, które mają osiągnąć barierę
static int threads_waiting;   // Liczba wątków, które już dotarły do bariery
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Muteks do synchronizacji
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;   // Zmienna warunkowa


void bariera_init(int n) {
  threads_to_wait = n;  // Ustaw liczbę wątków do oczekiwania
  threads_waiting = 0;  // Na początku żaden wątek nie czeka
} 

void bariera(void) {
  pthread_mutex_lock(&mutex);
  threads_waiting++; // Zwiększ licznik wątków oczekujących

  if (threads_waiting == threads_to_wait) {
    pthread_cond_broadcast(&cond); // Obudź wszystkie oczekujące wątki
  } else {
    // Wątek czeka, aż wszystkie inne dotrą do bariery
    while (threads_waiting < threads_to_wait) {
      pthread_cond_wait(&cond, &mutex);
    }
  }
  pthread_mutex_unlock(&mutex);
}