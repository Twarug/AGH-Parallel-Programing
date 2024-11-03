#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
  pthread_t thread;
  int id;
  double a, b, dx;

  double calka;
} ThreadData;

double funkcja ( double x );

double calka_sekw(double a, double b, double dx);

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w);

void* calka_podobszar_w(void* arg_wsk);

double calka_dekompozycja_obszaru(double a, double b, double dx, int l_w){


  //printf("a %lf, b %lf, dx %lf\n", a, b, dx);

  double calka_suma_local = 0.0;

  // tworzenie struktur danych do obsługi wielowątkowości
  ThreadData* data = malloc(l_w * sizeof(ThreadData));

  double d = (b - a) / l_w;

  // tworzenie wątków
  for (int i = 0; i < l_w; i++) {
    data[i].id = i;
    data[i].a = a + d * i;
    data[i].b = a + d * (i + 1);
    data[i].dx = dx;

    pthread_create(&data[i].thread, NULL, calka_podobszar_w, &data[i]);
  }

  // oczekiwanie na zakończenie pracy wątków
  for (int i = 0; i < l_w; i++) {
#if 1 // 5.0
    pthread_join(data[i].thread, NULL);
    calka_suma_local += data[i].calka;
#else
    double* result;
    pthread_join(data[i].thread, &result);

    calka_suma_local += *result;

    free(result);
#endif
  }

  free(data);

  return(calka_suma_local);
}



void* calka_podobszar_w(void* arg_wsk){
  ThreadData* data = arg_wsk;

  double a_local = data->a, b_local = data->b, dx = data->dx;
  // rozpakowanie danych przesłanych do wątku

  int my_id = data->id; // skąd pobierany?
  // printf("\nWątek %d: a_local %lf, b_local %lf, dx %lf\n", 
	//  my_id, a_local, b_local, dx);
  // wywołanie zadania do wykonania: całkowanie w zadanym przedziale 
  data->calka = calka_sekw(a_local, b_local, dx);
  double* calka = malloc(sizeof(double));
  *calka = data->calka;
  pthread_exit(calka);
  // (trzeba także dodać prototyp na początku pliku)
  
// zadanie w funkcji calka_sekw jest równoważne obliczeniom:
//  int N_local = ceil((b_local-a_local)/dx);
//  double dx_adjust_local = (b_local-a_local)/N_local;
//  printf("a_local %lf, b_local %lf, dx_adjust_local %lf, n_local %d\n", 
//	 a_local, b_local, dx_adjust_local, N_local);
//  int i;
//  double calka = 0.0;
//  for(i=0; i<N_local; i++){
//    double x1 = a_local + i*dx_adjust_local;
//    calka += 0.5*dx_adjust_local*(funkcja(x1)+funkcja(x1+dx_adjust_local));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);
//  }

}
