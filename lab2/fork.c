#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "pomiar_czasu.h"

int zmienna_globalna = 0;

int main() {

  int pid, wynik, i;

  inicjuj_czas();

  for (i = 0; i < 1000; i++) {

    pid = fork();

    if (pid == 0) {

      zmienna_globalna++;

      char *args[] = {"./program", "Patryk", "Twardosz", NULL};
      wynik = execv(args[0], args);

      if (wynik == -1)
        printf("Proces potomny nie wykonal programu\n");

      exit(0);

    } else {

      wait(NULL);
    }
  }

  printf("Zmienna globalna: %d\n", zmienna_globalna);

  drukuj_czas();
}
