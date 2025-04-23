#include "custom.h"

int main() {
    struct sockaddr_in b_addr;
    char buff[MAX];
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int enable = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));

    memset(&b_addr, 0, sizeof(b_addr));
    b_addr.sin_family = AF_INET;
    b_addr.sin_port = htons(PORT_NO);
    b_addr.sin_addr.s_addr = INADDR_BROADCAST;

    

    while (true) {
        memset(buff, 0, sizeof(buff));
        printf("Enter message (or type QUIT to exit): ");
        fgets(buff, MAX, stdin);

        sendto(sock_fd, buff, MAX, 0,
               (struct sockaddr*)&b_addr, sizeof(b_addr));

        if (strncmp(buff, "QUIT", 4) == 0)
            break;
    }

    printf("SENDER: Broadcast terminated...\n");
    close(sock_fd);
    return 0;
}

