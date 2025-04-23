#include "custom.h"

void main() {
	int sock_fd=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in server_addr,client_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(PORT_NO);
	while(true) {
		char buff[MAX];
		printf("Enter the Dataword: ");
		scanf(" %[^\n]s",buff);
		int l=sizeof(server_addr);
		sendto(sock_fd,buff,MAX,0,(struct sockaddr*)&server_addr,l);
		if(!strncmp(buff, "exit", 4)) break;
		recvfrom(sock_fd,buff,MAX,0,(struct sockaddr*)&server_addr,&l);
		printf("Message from server (Codeword): %s\n", buff);
	}
	close(sock_fd);
}
