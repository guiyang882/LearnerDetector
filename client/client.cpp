//
// Created by 贵阳 on 2017/4/18.
//

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

const int BUFSIZE = 4096;
char recvBuff[BUFSIZE], sendBuff[BUFSIZE];

int main(int argc, char *argv[])  {
    int sockfd = 0, n = 0;

    memset(recvBuff, '0',sizeof(recvBuff));
    memset(sendBuff, '0',sizeof(sendBuff));

    if(argc < 2) {
        printf("\n Usage: %s ip port \n",argv[0]);
        return 0;
    }

    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return 0;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    char buf[1024] = {'0'};
    while(true) {
        read(STDIN_FILENO, buf, sizeof(buf));
        if(sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
            ;
        }
    }
    return 0;
}
