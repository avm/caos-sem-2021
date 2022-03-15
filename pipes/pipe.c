#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int pipefd[2];
  pipe2(pipefd, O_CLOEXEC);
  if (fork() == 0) {  // yes
    close(pipefd[0]);
    dup2(pipefd[1], 1);
    execlp("yes", "yes", NULL);
    perror("!");
  }
  if (fork() == 0) {  // head
    dup2(pipefd[0], 0);
    execlp("head", "head", NULL);
    perror("!!!");
  }
  close(pipefd[0]);
  close(pipefd[1]);
  wait(NULL);
  wait(NULL);
}
