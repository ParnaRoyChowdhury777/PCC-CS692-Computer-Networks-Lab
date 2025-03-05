#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/un.h>
#include<sys/socket.h>
#include<ctype.h>

#define SOCKET_PATH "/tmp/mySocket"
#define BUFF_SIZE 16

int isValidIp(char *ip) {
	if(ip == NULL) return 0;
	
	char copy[BUFF_SIZE];
	strncpy(copy, ip, BUFF_SIZE - 1);
	copy[BUFF_SIZE - 1] = '\0';
	
	if(copy[0] == '.' || copy[strlen(copy) - 1] == '.')
		return 0;
	int count = 0;
	for(int i=0; copy[i]!='\0'; i++) {
		if(copy[i] == '.')
			count++;
	}
	
	if(count != 3)
		return 0;
	
	int num, dots = 0;
	char *token = strtok(copy, ".");
	
	while(token) {
		for(int i=0; token[i]!='\0'; i++) {
			if(!isdigit(token[i]))
				return 0;
		}
		num = atoi(token);
		if(num < 0 || num > 255)
			return 0;
		if(token[0] == '0' && strlen(token) > 1)
			return 0;
		token = strtok(NULL, ".");
		dots++;
	}
	
	return (dots == 4) ? 1 : 0;
}

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
		
		printf("Received ip address is: %s\n", buff);
		
		if(!strcmp(buff, "end")) {
			printf("Terminated\n");
			break;
		}
		
		int result = isValidIp(buff);
		
		int n = write(client_fd, &result, sizeof(result));
		if(n < 0) {
			perror("SERVER: Write failed");
			exit(0);
		}
		
		printf("Sent %d to client\n", result);
		memset(buff, 0, sizeof(buff));
		
	}
	
	close(client_fd);
	close(server_fd);
	unlink(SOCKET_PATH);
	return 0;
}
