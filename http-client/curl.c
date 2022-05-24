#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>  
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[]) {
    if (argc != 3) {
        puts("invalid argument count");
        exit(1);
    }
    struct addrinfo* res;
    int resp;
    if (resp = getaddrinfo(argv[1], "http", NULL, &res)) {
        puts(gai_strerror(resp));
        exit(1);
    }
    int fd = -1;
    for (struct addrinfo* current = res; current; current = current->ai_next) {
        fd = socket(current->ai_family, SOCK_STREAM, 0);
        if (fd == -1) {
            continue;
        }
        if (connect(fd, current->ai_addr, current->ai_addrlen)) {
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
    dprintf(fd, "GET %s HTTP/1.0\r\n\r\n", argv[2]);
    char buff[100];
    ssize_t buff_count;
    while ((buff_count = read(fd, buff, sizeof(buff))) > 0) {
        fwrite(buff, 1, buff_count, stdout);
    }
    printf("\n");
    fflush(stdout);
    close(fd);
}
