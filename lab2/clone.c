#define _GNU_SOURCE
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

#include "pomiar_czasu.h"

static int zmienna_globalna = 0;

int funkcja_watku(void *argument) {
  int *var = (int *)argument;

  for (int i = 0; i < 100000; i++) {
    zmienna_globalna++;
  }
  return 0;
}


#define ROZMIAR_STOSU 1024*64

int main()
{

  void *stos;
  pid_t pid;
  int i; 
  
  stos = malloc( ROZMIAR_STOSU );
  if (stos == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  inicjuj_czas();

  for(i=0;i<1000;i++){

    pid = clone( &funkcja_watku, (void *) stos+ROZMIAR_STOSU, 
		 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, 0 );

    waitpid(pid, NULL, __WCLONE);

  }

  printf("Zmienna globalna: %d\n",zmienna_globalna);
  drukuj_czas();

  free( stos );
}
