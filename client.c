#include "custom.h"

int main() {
	struct sockaddr_un client_addr;
	int client_len;
	char buff[BUFF_SIZE];
	
	int client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(client_fd == -1) {
		perror("client: Socket cannot be created");
		exit(0);
	}
	printf("Client: Socket created\n");
	
	client_len = sizeof(client_addr);
	memset(&client_addr, 0, client_len);
	client_addr.sun_family = AF_UNIX;
	strcpy(client_addr.sun_path, SOCKET_PATH);
	
	
	if(connect(client_fd, (struct sockaddr*)&client_addr, client_len) < 0) {
		perror("client: connect failed");
		exit(0);
	}
	
	while(true) {
		printf("Enter the bit stream: \n");
		scanf("%s", buff);
		
		int n = write(client_fd, buff, strlen(buff));
		if(n < 0) {
			perror("client: Write failed");
			exit(0);
		}
		
		if(!strcmp(buff, "end")) {
			printf("Terminated\n");
			break;
		}
		
		int m = read(client_fd, buff, sizeof(buff));
		if(m < 0) {
			perror("client: Read failed");
			exit(0);
		}
		
		buff[m] = '\0';
		
		printf("Modified bit stream is: %s\n", buff);
	}
		
		close(client_fd);
	
	return 0;
}
