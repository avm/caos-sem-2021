#include <unistd.h>
#include <stdio.h>

int main() {
  int pipefd[2];
  if (pipe(pipefd) == -1) {
    perror("pipe error");
    return 1;
  }

  int input = pipefd[0];
  int output = pipefd[1];
  if (fork()) {
    close(input);
    printf("2\n");
    int cur = 3;
    while (1) {
      write(output, &cur, sizeof(cur));
      cur += 2;
    }
  }
  close(output);
  int it = 0;
  while (it < 1000) {
    it++;
    int prime;
    read(input, &prime, sizeof(prime));
    printf("%d\n", prime);
    if (pipe(pipefd) == -1) {
      perror("pipe error");
      return 1;
    }
 
    if (fork()) {
      // close(output);
      output = pipefd[1];
      close(pipefd[0]);
      // parent
      while (1) {
        int cur;
        read(input, &cur, sizeof(cur));
        if (cur % prime != 0) {
          write(output, &cur, sizeof(cur));
        }
      }
    }
    close(input);
    close(pipefd[1]);
    input = pipefd[0];
  }

  return 0;
}
