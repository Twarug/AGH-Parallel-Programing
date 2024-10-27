#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

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
    (*var)++;
  }
  return 0;
}


#define ROZMIAR_STOSU 1024 * 64

int main() {

  void *stos1, *stos2;
  pid_t pid1, pid2;
  int local1 = 0, local2 = 0;

  stos1 = malloc(ROZMIAR_STOSU);
  stos2 = malloc(ROZMIAR_STOSU);
  if (stos1 == 0 || stos2 == 0) {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit(-1);
  }

  pid1 = clone(funkcja_watku, (void *) stos1 + ROZMIAR_STOSU, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &local1);
  pid2 = clone(funkcja_watku, (void *) stos2 + ROZMIAR_STOSU, CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &local2);

  waitpid(pid1, NULL, __WCLONE);
  waitpid(pid2, NULL, __WCLONE);

  free(stos1);
  free(stos2);

  printf("Zmienna globalna: %d\n", zmienna_globalna);
  printf("Local1: %d\n", local1);
  printf("Local2: %d\n", local2);
}
