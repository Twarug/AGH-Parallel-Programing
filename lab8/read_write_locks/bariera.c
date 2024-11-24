#include <pthread.h>
#include <stdio.h>

typedef struct bariera_t {
    int liczba_oczekujacych;
    int liczba_wymagana;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} bariera_t;

bariera_t bariera_init(int n) {
  bariera_t bariera;
  bariera.liczba_oczekujacych = 0;
  bariera.liczba_wymagana = liczba_wymagana;
  pthread_mutex_init(&bariera.mutex, NULL);
  pthread_cond_init(&bariera.cond, NULL);
  return bariera;
} 

void bariera(bariera_t* bariera) {
  pthread_mutex_lock(&bariera->mutex);
  bariera->liczba_oczekujacych++;
  if (bariera->liczba_oczekujacych < bariera->liczba_wymagana) {
      pthread_cond_wait(&bariera->cond, &bariera->mutex);
  } else {
      bariera->liczba_oczekujacych = 0; // Reset liczby oczekujących
      pthread_cond_broadcast(&bariera->cond); // Obudzenie wszystkich
  }
  pthread_mutex_unlock(&bariera->mutex);
}