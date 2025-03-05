#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/un.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>

#define SOCKET_PATH "/tmp/mySocket"
#define BUFF_SIZE 30

typedef struct {
    char dw[BUFF_SIZE];
    char gen[BUFF_SIZE];
} Word;
