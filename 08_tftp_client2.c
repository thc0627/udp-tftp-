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

#define ERROR_LOG(errorlog) do{\
                                perror(errorlog);\
                                printf("%s--%s--%d\n", __FILE__, __func__, __LINE__);\
                                exit(1);\
                              }while(0)

int main(int argc, char const *argv[])
{
    int sockfd = 0;
    struct sockaddr_in serveraddr;
    

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        ERROR_LOG("fail to socket");
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
    serveraddr.sin_port = htons(69);

    system("clear");
    printf("--------------------------\n");
    printf("--- 请输入help查看选项 ---\n");
    printf("--------------------------\n");

    

    return 0;
}
