#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <threads.h>

#define ILE_MUSZE_WYPIC 3

int dostepne_kufle;


void* watek_klient(void* arg);

int main(void) {
  pthread_t *tab_klient;
  int *tab_klient_id;
  int l_kl, l_kf, l_kr;

#if 0
  printf("\nLiczba klientow: "); scanf("%d", &l_kl);
  printf("\nLiczba kufli: "); scanf("%d", &l_kf);

  //printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  l_kr = 1000000000; // wystarczająco dużo, żeby nie było rywalizacji
#else
  l_kl = 10000;
  l_kf = 100;
  l_kr = 1000000000;
#endif

  dostepne_kufle = l_kf;

  tab_klient = (pthread_t*)malloc(l_kl * sizeof(pthread_t));
  tab_klient_id = (int*)malloc(l_kl * sizeof(int));
  for(int i=0; i < l_kl; i++) tab_klient_id[i] = i;

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf); 

  for(int i = 0; i < l_kl; i++)
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]); 

  for(int i = 0; i < l_kl; i++)
    pthread_join( tab_klient[i], NULL);

  int roznica_kufli = dostepne_kufle - l_kf;
  if (roznica_kufli != 0)
    fprintf(stderr, "\nRóznica w liczbie kufili wynosi: %d\n", roznica_kufli);
  printf("\nZamykamy pub!\n");
}

void do_something_else_or_nothing() {
  nanosleep((struct timespec[]){{0, 50000000L}}, NULL);
}

int wybierzKufel() {
  dostepne_kufle--;
}

void oddajKufel(int kufel) {
  dostepne_kufle++;
}

void* watek_klient(void* arg_wsk) {
  int moj_id = *((int*)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = ILE_MUSZE_WYPIC;

  long int wykonana_praca = 0;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id); 
    
  for(i=0; i<ile_musze_wypic; i++){

    printf("\nKlient %d, wybieram kufel\n", moj_id); 
    kufel = wybierzKufel();

    j=0;
    printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j); 
    usleep(30);
    
    printf("\nKlient %d, pije\n", moj_id);
    nanosleep((struct timespec[]){{0, 50000000L}}, NULL);
    
    printf("\nKlient %d, odkladam kufel\n", moj_id);
    oddajKufel(kufel);
  }

  printf("\nKlient %d, wychodzę z pubu; wykonana praca %ld\n", moj_id, wykonana_praca);
    
  return(NULL);
} 


