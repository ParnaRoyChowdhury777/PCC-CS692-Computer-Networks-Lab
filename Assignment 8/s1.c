#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MY_PORT 7000
#define FRIEND_PORT 7001

void *readMessage(void *arg);
void *writeMessage(void *arg);

int main() {
    int sockfd;
    struct sockaddr_in my_addr, friend_addr;
    pthread_t reader, writer;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MY_PORT);
    my_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));

    friend_addr.sin_family = AF_INET;
    friend_addr.sin_port = htons(FRIEND_PORT);
    friend_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    pthread_create(&reader, NULL, readMessage, &sockfd);
    pthread_create(&writer, NULL, writeMessage, &friend_addr);

    pthread_join(reader, NULL);
    pthread_join(writer, NULL);

    close(sockfd);
    return 0;
}

void *readMessage(void *arg) {
    int sockfd = *((int *)arg);
    struct sockaddr_in sender_addr;
    int addr_len = sizeof(sender_addr);
    char buffer[BUFFER_SIZE];

    while (1) {
        recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&sender_addr, &addr_len);
        printf("Friend: %s", buffer);
        if(!strncmp(buffer,"exit",4)) exit(0);
    }
    return NULL;
}

void *writeMessage(void *arg) {
    struct sockaddr_in friend_addr = *((struct sockaddr_in *)arg);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[BUFFER_SIZE];

    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&friend_addr, sizeof(friend_addr));
        if(!strncmp(buffer,"exit",4)) exit(0);
    }
    return NULL;
}

