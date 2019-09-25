#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    int sockfd = 0;
    char text[64] = "";
    socklen_t addrlen = sizeof(struct sockaddr_in);
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sockfd < 0)
    {
        perror("socket");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(atoi(argv[2]));

    if(bind(sockfd, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        perror("bind");
        exit(1);
    }

    while(1)
    {
        if(recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&clientaddr, &addrlen) < 0)
        {
            perror("recvfrom");
            exit(1);
        }

        printf("[%s-%d]:%s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), text);

        strcat(text, "*_*");

        if(sendto(sockfd, text, sizeof(text), 0, (struct sockaddr *)&clientaddr, addrlen) < 0)
        {
            perror("sendto");
            exit(1);
        }
    }
    
    return 0;
}
