#include "custom.h"


char *bit_stuffing(char *input) {
	char *output = (char*)malloc(MAX*sizeof(char));
	int i, j=0, count = 0;
	int flag = 0;
	for(i=0; i<strlen(input); i++) {   
		output[j++] = input[i];
		if(input[i] == '1') {
			count++;
			if(count == 5 && flag) {
				output[j++] = '0';
				count = 0;
				flag = 0;
			}
		} else {
			count = 0;
			flag = 1;
		}
		
	}
	output[j] = '\0';
	return output;
}

int main() {
	char buffer[MAX];
	int server_fd, client_fd, server_len, client_len;
	struct sockaddr_in server_addr, client_addr;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0) {
		perror("Server socket creation failed");
		exit(0);
	}
	
	server_len = sizeof(server_addr);
	client_len = sizeof(client_addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT_NO);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(&buffer, 0, sizeof(buffer));
	
	if(bind(server_fd, (struct sockaddr*)&server_addr, server_len) == -1) {
		perror("Server bind failed");
		exit(0);
	}
	
	if(listen(server_fd, 1) < 0) {
		perror("Server listen failed");
		exit(0);
	}
	
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	if(client_fd < 0) {
		perror("Server accept failed");
		exit(0);
	}
	
	while(1) {
		printf("Server waiting\n");
		if(read(client_fd, buffer, sizeof(buffer)) == -1) {
			perror("Server read failed");
			exit(0);
		}
		printf("Data received is : %s\n", buffer);
		if(!strncmp(buffer, "exit", 4)) {
			printf("Program terminated\n");
			break;
		}
		char modified[MAX];
		strcpy(modified ,bit_stuffing(buffer));
		if(write(client_fd, modified, strlen(modified)) < 0) {
			perror("Server write failed");
			exit(0);
		}
		memset(&buffer, 0, sizeof(buffer));
	}
	close(client_fd);
	close(server_fd);
	
	return 0;
}
