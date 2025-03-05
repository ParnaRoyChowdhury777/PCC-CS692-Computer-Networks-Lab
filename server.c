#include "custom.h"

int main() {
	struct sockaddr_un server_addr, client_addr;
	int server_len, client_len;
	char buff[BUFF_SIZE];
	unlink(SOCKET_PATH);
	
	int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(server_fd == -1) {
		perror("SERVER: Socket cannot be created");
		exit(0);
	}
	printf("SERVER: Socket created\n");
	
	server_len = sizeof(server_addr);
	memset(&server_addr, 0, server_len);
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, SOCKET_PATH);
	
	if(bind(server_fd, (struct sockaddr*)&server_addr, server_len) < 0) {
		perror("SERVER: Bind failed");
		exit(0);
	}
	
	if(listen(server_fd, 20) < 0) {
		perror("SERVER: Listen failed");
		exit(0);
	}
	
	client_len = sizeof(client_addr);
		int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
		if(client_fd < 0) {
			perror("SERVER: Accept failed");
			exit(0);
		}
		
	
	while(true) {
		printf("Server waiting\n");
		
		int m = read(client_fd, buff, sizeof(buff));
		if(m < 0) {
			perror("SERVER: Read failed");
			exit(0);
		}
		
		printf("Received bit stream is: %s\n", buff);
		
		if(!strcmp(buff, "end")) {
			printf("Terminated\n");
			break;
		}
		
		int parity = 0;
		for(int i=0; i<m; i++) {
			if(buff[i] == '1')
				parity++;
		}
		buff[m] = (parity % 2) ? '1' : '0';
		buff[m+1]='\0';
		
		printf("Modified bit stream is: %s\n", buff);
		
		int n = write(client_fd, buff, strlen(buff));
		if(n < 0) {
			perror("SERVER: Write failed");
			exit(0);
		}
		
		
	}
	memset(buff, 0, sizeof(buff));
		close(client_fd);
	close(server_fd);
	unlink(SOCKET_PATH);
	return 0;
}
