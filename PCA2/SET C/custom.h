#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<math.h>

#define MAX 1024
#define PORT 8080
#define ip "127.0.0.1"

typedef struct {
	char generator[MAX];
	char dataword[MAX];
}Word;