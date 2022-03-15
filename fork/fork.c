#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    FILE *f = fopen("file", "w");
    int child = fork();
    if (child == 0) { // child
        int fd = fileno(f);
        dup2(fd, STDOUT_FILENO);
        execlp("echo", "something", "Hello from exec", NULL);
        perror("execlp");
        _exit(1);
        fprintf(f, "Hello from child\n");
        fseek(f, 0, SEEK_SET);
        return 0;
    }
    waitpid(child, NULL, 0);
    fprintf(f, "Hello from p\n");
}

