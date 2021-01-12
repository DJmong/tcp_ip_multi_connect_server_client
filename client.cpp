#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
#include <stdlib.h>

#include "format.h"



int main(int argc, char **argv)
{
    struct sockaddr_in serveraddr;
    int server_sockfd;
    int client_len;
    int seq = 1;
    data a;

    if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("error: ");
        return 0;    
    }

    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(12345);

    client_len = sizeof(serveraddr);

    if(connect(server_sockfd, (struct sockaddr *)&serveraddr, client_len) == -1)
    {
        perror("connect error: ");
        return 1;
    }
    
    system("clear");
    while(1)
    {
        memset(&a, 0, sizeof(struct data));
        read(0, a.buf, BUF_SIZE);
        a.seq = seq;

        printf("write seq : %d write ack : %d \n", a.seq, a.ack);
        write(server_sockfd, &a, sizeof(struct data));

        memset(&a, 0, sizeof(struct data));        
        read(server_sockfd, &a, sizeof(struct data));
        printf("read  seq : %d read  ack : %d \n", a.seq, a.ack);

        if(a.ack == (a.seq + 1))
        {
            a.seq = 0;      
  
            send(server_sockfd, &a, sizeof(struct data), 0);
            printf("write seq : %d write ack : %d \n", a.seq, a.ack);            
            printf("***send success***\n");

            memset(&a, 0, sizeof(struct data));
            read(server_sockfd, &a, sizeof(struct data));

            printf("==========result=========\n");
            printf("read  seq : %d read  ack : %d \n", a.seq, a.ack);
            printf("=========================\n");
        }
        else
        {
            printf("error!");
            exit(1);
        }
        sleep(1);
        seq++;
    }
    close (server_sockfd);
   
    return 0;
}
