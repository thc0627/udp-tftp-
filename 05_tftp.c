#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int sockfd;
struct sockaddr_in serveraddr;
socklen_t addrlen = sizeof(struct sockaddr_in);
unsigned char name[128] = "";

void receive_fun(void)
{
    unsigned char ret[520] = "";
    unsigned int fd = 0;
    unsigned int num = 0;
    unsigned int flag = 0;
    unsigned int types = 0;

    while (1)
    {
        if((types = recvfrom(sockfd, ret, sizeof(ret), 0, (struct sockaddr *)&serveraddr, &addrlen)) < 0)
        {
            perror("recvfrom");
            exit(1);
        }

        if(ret[1] == 3)
        {
            if(num+1 == ntohs(*(unsigned short *)(ret+2)) && types == 516)
            {
                num = ntohs(*(unsigned short *)(ret+2));
                if(flag == 0)
                {
                    fd = open(name, O_RDWR|O_CREAT|O_TRUNC, 0666);
                    flag = 1;
                }

                write(fd, ret+4, types-4);

                ret[1] = 4;
                if(sendto(sockfd, ret, 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
                {
                    perror("sendto");
                    exit(1);
                }
            }
            else if (types < 516)
            {
                write(fd, ret+4, types-4);

                ret[2] = 4;
                if(sendto(sockfd, ret, 4, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
                {
                    perror("sendto");
                    exit(1);
                }

                break;
            } 
        }
        else if (ret[1] == 5)
        {
            printf("error\n");
            exit(1);
        }
    }

    printf("下载完成\n");
    close(fd);
    exit(0);

    return;
}

int main(int argc, char const *argv[])
{
    int types = 0;
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(69);

    unsigned char msg[32] = "";
    printf("请输入要下载文件的名字:");
    fgets(name, sizeof(name), stdin);
    name[strlen(name) - 1] = 0;

    types = sprintf(msg, "%c%c%s%c%s%c", 0, 1, name, 0, "octet", 0);

    if(sendto(sockfd, msg, types, 0, (struct sockaddr *)&serveraddr, addrlen) < 0)
    {
        perror("sendto");
        exit(1);
    }

    receive_fun();

    return 0;
}
