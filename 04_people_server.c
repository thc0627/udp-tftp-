#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

typedef struct 
{
    char ip[16];
    short port;
    char text[128];
}MSG;

int main(int argc, char const *argv[])
{
    int sockfd;
    socklen_t addrlen = sizeof(struct sockaddr_in);

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        perror("bind");
        exit(1);
    }

    MSG msg;
    struct sockaddr_in sendaddr;
    struct sockaddr_in recvdaddr;


    while (1)
    {
        if(recvfrom(sockfd, &msg, sizeof(MSG), 0, (struct sockaddr *)&sendaddr, &addrlen) < 0)
        {
            perror("recvfrom");
            exit(1);
        }

        recvdaddr.sin_family = AF_INET;
        recvdaddr.sin_addr.s_addr = inet_addr(msg.ip);
        recvdaddr.sin_port = htons(msg.port);

        strcpy(msg.ip, inet_ntoa(sendaddr.sin_addr));
        msg.port = ntohs(sendaddr.sin_port);

        if(sendto(sockfd, &msg, sizeof(MSG), 0, (struct sockaddr *)&recvdaddr, addrlen) < 0)
        {
            perror("sendto");
            exit(1);
        }
    }
    
    return 0;
}
