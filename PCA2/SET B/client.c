#include "custom.h"

int main() {
	int sock_fd;
	char buff[MAX];
	struct sockaddr_in server_addr;
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0) {
		perror("Client Socket failed\n");
		exit(0);
	}
	memset(buff,0,sizeof(buff));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(ip);
	
	while(1) {
		printf("Enter the dataword: \n");
		scanf("%s",buff);
		sendto(sock_fd, buff, MAX, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
		if(!strncmp(buff, "exit", 4)) {
			printf("Client terminated\n");
			break;
		}
		int l = sizeof(server_addr);
		recvfrom(sock_fd, buff, MAX, 0, (struct sockaddr*)&server_addr, &l);
		printf("Received codeword is : %s\n", buff);
		memset(buff, 0, sizeof(buff));
	}
	
	
	close(sock_fd);
	return 0;
}