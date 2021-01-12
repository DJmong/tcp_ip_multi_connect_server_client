#ifndef FORMAT_H
#define FORMAT_H

#define BUF_SIZE 256

struct data
{
    int seq;
    char buf[BUF_SIZE];
    int ack;
};

#endif
