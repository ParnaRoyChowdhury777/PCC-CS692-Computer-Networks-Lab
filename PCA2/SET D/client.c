#include "custom.h"

char* bit_stuffing(char *input) {
	char* output = (char*)malloc(MAX*sizeof(char));
	int j=0, count=0, flag=0;
	for(int i=0; i<strlen(input); i++) {
		output[j++]=input[i];
		if(input[i]=='1') {
			count++;
			if(count == 5 && flag) {
				output[j++]='0';
				count = 0;
				flag = 0;
			}
		} else {
			count = 0;
			flag = 1;
		}
	}
	output[j]='\0';
	return output;
}

int main() {
	int sock_fd;
	char buff[MAX];
	struct sockaddr_in r_addr;
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(buff, 0, sizeof(buff));
	
	int enable = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
	
	memset(&r_addr, 0, sizeof(r_addr));
	r_addr.sin_family=AF_INET;
	r_addr.sin_port=htons(PORT);
	r_addr.sin_addr.s_addr = INADDR_ANY;
	
	bind(sock_fd ,(struct sockaddr*)&r_addr, sizeof(r_addr));
	
	while(1) {
		recvfrom(sock_fd, buff, MAX, 0, NULL, NULL);
		printf("Dataword: %s\n", buff);
		if(!strncmp(buff, "QUIT", 4)) {
			printf("Terminated\n");
			break;
		}
		printf("Modified bit stuffed word: %s\n", bit_stuffing(buff));
		memset(buff,0,sizeof(buff));
	}
	
	close(sock_fd);
	return 0;
}