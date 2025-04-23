#include "custom.h"

int main() {
	char buffer[MAX];
	int client_fd, client_len;
	struct sockaddr_in client_addr;
	
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(client_fd < 0) {
		perror("Client socket creation failed");
		exit(0);
	}
	
	client_len = sizeof(client_addr);
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT_NO);
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&buffer, 0, sizeof(buffer));
	
	if(connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
		perror("Client connect failed");
		exit(0);
	}
	
	while(1) {
		printf("Enter the data: \n");
		scanf("%s", buffer);
		
		if(write(client_fd, buffer, strlen(buffer)) < 0) {
			perror("client write failed");
			exit(0);
		}
		
		if(!strncmp(buffer, "exit", 4)) {
			printf("Program terminated\n");
			break;
		}
		
		int m = read(client_fd, buffer, sizeof(buffer));
		if(m== -1) {
			perror("client read failed");
			exit(0);
		}
		buffer[m] = '\0';
		printf("Data received is : %s\n", buffer);
		
		
		memset(&buffer, 0, sizeof(buffer));
	}
	close(client_fd);
	
	return 0;
}
