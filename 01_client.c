#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
    int sockfd = 0;
    struct sockaddr_in severaddr;
    socklen_t addrlen = sizeof(severaddr);

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    severaddr.sin_family = AF_INET;
    severaddr.sin_addr.s_addr = inet_addr(argv[1]);
    severaddr.sin_port = htons(atoi(argv[2]));

    char buf[64] = "";
    while(1)
    {
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = 0;

        if((sendto(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&severaddr, addrlen)) < 0)
        {
            perror("sendto");
            exit(1);
        }

        char text[32] = "";
        if((recvfrom(sockfd, text, sizeof(text), 0, (struct sockaddr *)&severaddr, &addrlen)) < 0)
        {
            perror("recvfrom");
            exit(1);
        }

        printf("receive from server :%s\n", text);
    }

    close(sockfd);
    return 0;
}

