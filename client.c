#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SA  struct sockaddr
#define BUFSIZE 4096


int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char *buf[BUFSIZE];
    int read_n;

    if(argc != 3) {
        fprintf(stderr, "usage: client <server_address> <server_port>\n");
        exit(1);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        fprintf(stderr, "inet_pton error: %s\n", strerror(errno));
        exit(1);
    }
    if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr))) {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        exit(1);
    }
    while((read_n = read(sockfd, buf, BUFSIZE)) > 0) {
        if(write(fileno(stdout), buf, read_n) < 0) {
            fprintf(stderr, "write error: %s\n", strerror(errno));
            exit(1);
        }
    }
    if(read_n < 0) {
        fprintf(stderr, "read error: %s\n", strerror(errno));
        exit(1);
    }
    exit(0);
}
