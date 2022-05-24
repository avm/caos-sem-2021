#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>  
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        puts("invalid argument count");
        exit(1);
    }
    struct addrinfo* res;
    struct addrinfo hint = {.ai_family=AF_UNSPEC, .ai_protocol = IPPROTO_TCP, .ai_socktype = SOCK_STREAM};
    int resp;
    if (resp = getaddrinfo(NULL, argv[1], &hint, &res)) {
        puts(gai_strerror(resp));
        exit(1);
    }
    int fd = -1;
    for (struct addrinfo* current = res; current; current = current->ai_next) {
        fd = socket(current->ai_family, SOCK_STREAM, 0);
        if (fd == -1) {
            continue;
        }
        if (bind(fd, current->ai_addr, current->ai_addrlen) || listen(fd, 5)) {
            close(fd);
            fd = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if (fd == -1) {
        perror("No sockets?");
        return 1;
    }
    while (1) {
        struct sockaddr address;
        socklen_t address_len = sizeof(address);
        int connection = accept(fd, &address, &address_len);
        if (fork() == 0) {
            FILE* in = fdopen(connection, "r");
            char* method = NULL;
            char* path = NULL;
            char* protocol = NULL;
            fscanf(in, "%ms %ms %ms", &method, &path, &protocol);
            dprintf(connection, "HTTP/1.0 200 OK\r\n\r\nYou have asked for %s\r\nToo bad\r\n", path);
            free(method);
            free(path);
            free(protocol);
            fclose(in);
            return 0;
        }
        close(connection);
        while (waitpid(-1, NULL, WNOHANG) > 0) {
        }
    }
}
