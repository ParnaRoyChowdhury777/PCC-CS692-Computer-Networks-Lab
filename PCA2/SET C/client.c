#include "custom.h"

int main() {
	int client_fd, client_len;
	Word m;
	struct sockaddr_in client_addr;
	char result[MAX];
	
	client_fd = socket(AF_INET, SOCK_STREAM, 0);
	client_len = sizeof(client_addr);
	memset(&m, 0, sizeof(m));
	memset(result, 0, sizeof(result));
	client_addr.sin_family=AF_INET;
	client_addr.sin_port=htons(PORT);
	client_addr.sin_addr.s_addr=inet_addr(ip);
	
	connect(client_fd, (struct sockaddr*)&client_addr, client_len);
	while(1) {
		printf("Enter the dataword: \n");
		scanf("%s",m.dataword);
		printf("Enter the generator: \n");
		scanf("%s",m.generator);
		
		write(client_fd, &m, sizeof(m));
		if(!strncmp(m.dataword, "exit", 4) || !strncmp(m.generator, "exit", 4)) {
			printf("Client terminated\n");
			break;
		}
		int n = read(client_fd, result, sizeof(result));
		result[n]='\0';
		printf("The codeword is: %s\n",result);
		memset(&m, 0, sizeof(m));
		memset(result, 0, sizeof(result));
	}
	close(client_fd);
	return 0;
}