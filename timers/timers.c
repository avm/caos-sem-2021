#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t sigusr1_flag = 0;

void handler(int sig) {
    sigusr1_flag = 1;
}

int child(int lifetime, int fd_in) {
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);

    int cur;
    while (lifetime) {
        int x;
        int result = read(fd_in, &x, sizeof(x));
        if (result < 0 && errno != EINTR) {
            perror("read");
            exit(1);
        }
        if (result == 0) {
            printf("[proc %d]: terminated after result = 0\n", getpid());
            exit(0);
        }
        if (result == sizeof(x)) {
            cur = x;
        }
        if (sigusr1_flag) {
            printf("[proc %d]: left lifetime %d, number = %d\n", getpid(), lifetime, cur);
            --lifetime;
            sigusr1_flag = 0;
        }
    }
    printf("[proc %d]: terminated\n", getpid());
    exit(0);
}

void handle_sigpipe(int sig, siginfo_t *info, void* data) {
    char buf[] = "Received sigpipe\n";
    write(STDOUT_FILENO, buf, sizeof(buf) - 1);
    _exit(0);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Wrong syntax call\n");
        return 1;
    }
    
    struct sigaction sa;
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = handle_sigpipe;
    sigaction(SIGPIPE, &sa, NULL);
    int fd[2];
    pipe(fd);
    for (int i = 1; i < argc; ++i) {
        if (fork() == 0) {
            close(fd[1]);
            child(atoi(argv[i]), fd[0]);
        }
    }
    close(fd[0]);
    int x;
    while (scanf("%d", &x) == 1) {
        write(fd[1], &x, sizeof(x));
    }
    /*
    int proc_cnt = argc - 1;
    for (int i = 0; i < proc_cnt; ++i) {
        wait(NULL);
    }*/
}

