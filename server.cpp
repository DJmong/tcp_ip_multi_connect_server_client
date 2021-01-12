#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <stdlib.h>

#include "format.h"



int MakeChannel(int sockfd, int sockcl)
{
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size;
    int ack;
    data a;
    clientaddr_size = sizeof(clientaddr);

    if(sockcl == -1)
    {
        return 0;
    }
    while(1)
    {
        memset(&a, 0, sizeof(struct data));
        read(sockcl, &a, sizeof(struct data));
        printf("read  seq : %d read  ack : %d buf : %s\n", a.seq, a.ack, a.buf);
        ack = a.seq + 1;    
        a.ack = ack;

        printf("write seq : %d write ack : %d \n", a.seq, a.ack);

        write(sockcl, &a, sizeof(struct data));        
        
        memset(&a, 0, sizeof(struct data));
        read(sockcl, &a, sizeof(struct data));
        printf("read  seq : %d read  ack : %d \n", a.seq, a.ack);

        if(a.ack == ack)
        {
            a.seq = a.ack;
            a.ack = 0;
            write(sockcl, &a, sizeof(struct data));
            printf("***connect success***\n");
            printf("==========result=========\n");
            printf("write seq : %d write ack : %d \n", a.seq, a.ack);
            printf("=========================\n");
        }
        sleep(1);
    }

    close(sockcl);
}

int main(int argc, char **argv)
{
    int sockfd;
    int sockcl;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    socklen_t clientaddr_size;
    
    //make socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        std::cout << "error!" << std::endl;
        return 1;
    }
    memset(&serveraddr, 0x00, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl (INADDR_ANY);
    serveraddr.sin_port = htons(12345);

    //bind socket
    if(bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) == -1)
    {
        std::cout << "bind error!" << std::endl;
        return 1;
    }

    //listen signal
    if(listen(sockfd, 5) == -1)
    {
        std::cout << "listen error!" << std::endl;
        return 1;
     }
	
    system("clear");
    while(1){
        //accept the connect
        sockcl = accept(sockfd, (struct sockaddr*)&clientaddr, &clientaddr_size);
        
        //make thread, and do write and read
        std::thread *th = new std::thread(&MakeChannel, sockfd, sockcl);

    }
    close(sockfd);
    return 0;
}   
