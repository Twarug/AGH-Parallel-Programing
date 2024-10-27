
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int zmienna_wspolna=0;

#define WYMIAR 1000
#define ROZMIAR (WYMIAR*WYMIAR)
double a[ROZMIAR], b[ROZMIAR], c[ROZMIAR];

double czasozajmowacz(){
  int i, j, k;
  int n=WYMIAR;
  for(i=0;i<ROZMIAR;i++) a[i]=1.0*i;
  for(i=0;i<ROZMIAR;i++) b[i]=1.0*(ROZMIAR-i);
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
      c[i+n*j]=0.0;
      for(k=0;k<n;k++){
	c[i+n*j] += a[i+n*k]*b[k+n*j];
      }
    }
  }
  return(c[ROZMIAR-1]);
}

void* zadanie_watku(void * arg_wsk)
{
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	printf("  watek potomny: uniemozliwione zabicie\n");

	czasozajmowacz();

	printf("  watek potomny: umozliwienie zabicia\n");
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

	pthread_testcancel();

	zmienna_wspolna++;
	printf("  watek potomny: zmiana wartosci zmiennej wspolnej\n");

	return(NULL);
}

int main()
{
	pthread_t tid;
	pthread_attr_t attr;
	void *wynik;
	int i;

	// Initialize thread attributes
	pthread_attr_init(&attr);

	// Thread 1: Default properties (joinable)
	printf("watek glowny: tworzenie watku potomnego nr 1\n");
	pthread_create(&tid, NULL, zadanie_watku, NULL);

	sleep(2); // time for the thread to start

	printf("  watek glowny: wyslanie sygnalu zabicia watku\n");
	pthread_cancel(tid);

	// Join thread 1
	pthread_join(tid, &wynik);
	if (wynik == PTHREAD_CANCELED)
		printf("  watek glowny: watek potomny zostal zabity\n");
	else
		printf("  watek glowny: watek potomny NIE zostal zabity - blad\n");

	// Reset shared variable
	zmienna_wspolna = 0;

	// Thread 2: Default properties (joinable, then detached)
	printf("watek glowny: tworzenie watku potomnego nr 2\n");
	pthread_create(&tid, NULL, zadanie_watku, NULL);

	sleep(2); // time for the thread to start

	// Detach thread 2
	pthread_detach(tid);
	printf("  watek glowny: odlaczenie watku potomnego\n");

	printf("  glowny: wyslanie sygnalu zabicia watku odlaczonego\n");
	pthread_cancel(tid);

	// Thread 3: Detached state from creation
	printf("watek glowny: tworzenie odlaczonego watku potomnego nr 3\n");
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&tid, &attr, zadanie_watku, NULL);

	// Destroy attributes
	pthread_attr_destroy(&attr);

	printf("  watek glowny: koniec pracy, watek odlaczony pracuje dalej\n");
	pthread_exit(NULL); // main thread exits
}
