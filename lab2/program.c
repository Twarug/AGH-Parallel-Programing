#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if(argc > 2)
    printf("With args: PID: %d, %s %s\n", getpid(), argv[1], argv[2]);
  else
    printf("Without args: PID: %d\n", getpid());

  return 0;
}
