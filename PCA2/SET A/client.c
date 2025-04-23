#include "custom.h"

int main() {
	int client_fd, client_len;
	char buff[MAX];
	struct sockaddr_in client_addr;
	
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0) {
		perror("Client socket failed\n");
		exit(0);
	}
	client_len = sizeof(client_addr);
	memset(buff, 0, sizeof(buff));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
	client_addr.sin_addr.s_addr = inet_addr(ip);
	
	connect(client_fd, (struct sockaddr*)&client_addr, client_len);
	while(1) {
		printf("Enter the dataword: \n");
		scanf("%s",buff);
		
		write(client_fd, buff, strlen(buff));
		if(!strncmp(buff, "exit", 4)) {
			printf("Client Terminated\n");
			break;
		}
		int m = read(client_fd, buff, sizeof(buff));
		buff[m] = '\0';
		printf("Received codeword is: %s\n",buff);
		memset(buff, 0, sizeof(buff));
	}
	
	close(client_fd);
	return 0;
}