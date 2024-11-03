#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

double funkcja ( double x );

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w);

static int l_w_global=0;

static pthread_mutex_t mtx;
static double calka_global=0.0;

static double a_global;
static double b_global;
static double dx_global;
static int N_global;

void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w){

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

  calka_global = 0;

  l_w_global = l_w;
  a_global = a;
  b_global = b;
  dx_global = dx_adjust;
  N_global = N;

  printf("Obliczona liczba trapezów: N = %d, dx_adjust = %lf\n", N, dx_adjust);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);

  // tworzenie struktur danych do obsługi wielowątkowości
  pthread_t* threads = malloc(l_w * sizeof(pthread_t));
  int* ids = malloc(l_w * sizeof(int));

  pthread_mutex_init(&mtx, NULL);

  // tworzenie wątków
  for (int i = 0; i < l_w; i++) {
    ids[i] = i;

    ids[i] == *(ids + i * sizeof(int));

    i[ids]  == ids[i];

    pthread_create(&threads[i], NULL, calka_fragment_petli_w, &ids[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++)
    pthread_join(threads[i], NULL);

  pthread_mutex_destroy(&mtx);

  free(threads);
  free(ids);

  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

  int my_id = *(int*)arg_wsk;

  double a, b, dx; // skąd pobrać dane a, b, dx, N, l_w ? 
  int N, l_w;      // wariant 1 - globalne

  // a = a_global; // itd. itp. - wartości globalne nadaje calka_zrownoleglenie_petli

  a = a_global;
  b = b_global;
  dx = dx_global;
  N = N_global;
  l_w = l_w_global;

  int j = ceil((float)N / l_w);

#if 1 // 3.0
  // dekompozycja cykliczna
  int my_start = my_id;
  int my_end = N;
  int my_stride = l_w;
#else
  // dekompozycja blokowa
  int my_start = j * my_id;
  int my_end = j * (my_id + 1);
  int my_stride = 1;
#endif

  // something else ? (dekompozycja blokowo-cykliczna)
  if(j*l_w!=N) { printf("Error! Exiting.\n"); exit(0);}

  // printf("\nWątek %d: my_start %d, my_end %d, my_stride %d\n", 
	//  my_id, my_start, my_end, my_stride);


  int i;
  double calka = 0.0;
  for(i=my_start; i<my_end; i+=my_stride){

    double x1 = a + i*dx;
    calka += 0.5*dx*(funkcja(x1)+funkcja(x1+dx));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  } 
  
  pthread_mutex_lock(&mtx);
  calka_global += calka;
  pthread_mutex_unlock(&mtx);

  pthread_exit(NULL);
}

