#include "custom.h"

int main() {
	int sock_fd;
	char buff[MAX];
	struct sockaddr_in b_addr;
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(buff, 0, sizeof(buff));
	
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
	
	memset(&b_addr, 0, sizeof(b_addr));
	b_addr.sin_family=AF_INET;
	b_addr.sin_port=htons(PORT);
	b_addr.sin_addr.s_addr = INADDR_BROADCAST;
	
	while(1) {
		printf("Enter the dataword: \n");
		scanf("%s", buff);
		sendto(sock_fd, buff, MAX, 0, (struct sockaddr*)&b_addr, sizeof(b_addr));
		if(!strncmp(buff, "QUIT", 4)) {
			printf("Terminated\n");
			break;
		}
		memset(buff,0,sizeof(buff));
	}
	
	close(sock_fd);
	return 0;
}