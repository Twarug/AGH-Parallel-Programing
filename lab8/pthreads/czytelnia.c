#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"


/*** Implementacja procedur interfejsu ***/

int my_read_lock_lock(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->mutex);
  while (cz_p->l_p > 0) {
    pthread_cond_wait(&cz_p->czytelnicy_cond, &cz_p->mutex);
  }
  cz_p->l_c++;
  pthread_mutex_unlock(&cz_p->mutex);
}


int my_read_lock_unlock(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->mutex);
  cz_p->l_c--;
  if (cz_p->l_c == 0) {
    pthread_cond_signal(&cz_p->pisarze_cond);
  }
  pthread_mutex_unlock(&cz_p->mutex);
}


int my_write_lock_lock(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->mutex);
  while (cz_p->l_c > 0 || cz_p->l_p > 0) {
    pthread_cond_wait(&cz_p->pisarze_cond, &cz_p->mutex);
  }
  cz_p->l_p++;
  pthread_mutex_unlock(&cz_p->mutex);
}


int my_write_lock_unlock(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->mutex);
  cz_p->l_p--;
  pthread_cond_broadcast(&cz_p->czytelnicy_cond);
  pthread_cond_signal(&cz_p->pisarze_cond);
  pthread_mutex_unlock(&cz_p->mutex);
}

void inicjuj(cz_t* cz_p){
  cz_p->l_p = 0;
  cz_p->l_c = 0;

  pthread_mutex_init(&cz_p->mutex, NULL);
  pthread_cond_init(&cz_p->czytelnicy_cond, NULL);
  pthread_cond_init(&cz_p->pisarze_cond, NULL);
}

void czytam(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->mutex);
  printf("\t\t\t\t\tczytam:  l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p); 

  #if MY_DEBUG
  if (cz_p->l_p > 1 || (cz_p->l_p == 1 && cz_p->l_c > 0) || cz_p->l_p < 0 || cz_p->l_c < 0) {
    printf("Błąd: nieprawidłowy stan liczników!\n");
    pthread_mutex_unlock(&cz_p->mutex);
    exit(0);
  }
  #endif

  pthread_mutex_unlock(&cz_p->mutex);

  usleep(rand() % 3000000);
}

void pisze(cz_t* cz_p){
  pthread_mutex_lock(&cz_p->mutex);
  printf("\t\t\t\t\tpisze:   l_c %d, l_p %d\n", cz_p->l_c, cz_p->l_p);
  
  #if MY_DEBUG
  if (cz_p->l_p > 1 || (cz_p->l_p == 1 && cz_p->l_c > 0) || cz_p->l_p < 0 || cz_p->l_c < 0) {
    printf("Błąd: nieprawidłowy stan liczników!\n");
    pthread_mutex_unlock(&cz_p->mutex);
    exit(0);
  }
  #endif

  pthread_mutex_unlock(&cz_p->mutex);
  usleep(rand() % 3000000);
}


