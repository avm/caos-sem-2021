#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  int *number = mmap(NULL, 4, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  if (number == MAP_FAILED) {
    perror("mmap failed");
    return 1;
  }
  int f = fork();
  if (f < 0) {
    perror("reproduction failed");
    return 1;
  }
  if (f == 0) {  // son
    for (int i = 0; i < 10; ++i) {
      sleep(1);
      printf("read: %d\n", *number);
    }
    return 0;
  } else {  // parent
    for (int i = 0; i < 10; ++i) {
      *number = i;
      printf("write: %d\n", *number);
      sleep(1);
    }
  }
}
