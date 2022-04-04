#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SA  struct sockaddr
#define BUFSIZE 4096
#define LISTENQ 1024


int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;
    int file_fd;
    char buf[BUFSIZE];
    int read_n;

    if(argc != 3) {
        fprintf(stderr, "usage: server <port> <filename>\n");
        exit(1);
    }

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(1);

    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    if(bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        exit(1);
    }
    if(listen(listenfd, LISTENQ)) {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        exit(1);
    }
    for(;;) {
        clilen = sizeof(cliaddr);
        if((connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0) {
            fprintf(stderr, "accept error: %s\n", strerror(errno));
            exit(1);
        }
        if((file_fd = open(argv[2], O_RDONLY)) < 0) {
            fprintf(stderr, "open error: %s\n", strerror(errno));
            exit(1);
        }
        while((read_n = read(file_fd, buf, BUFSIZE)) > 0) {
            if(write(connfd, buf, read_n) < 0) {
                fprintf(stderr, "write error: %s\n", strerror(errno));
                break;
            }
        }
        if(read_n < 0) {
            fprintf(stderr, "read error: %s\n", strerror(errno));
        }
        if(shutdown(connfd, SHUT_WR) < 0) {
            fprintf(stderr, "shutdown error: %s\n", strerror(errno));
        }
    }
    exit(0);
}
