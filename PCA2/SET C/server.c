#include "custom.h"

void xorDiv(char* dividend, char* divisor, int n, int m) {
	for(int i=0; i<=n-m; i++) {
		if(dividend[i] == '1') {
			for(int j=0; j<m; j++) {
				dividend[i+j] = (dividend[i+j] == divisor[j]) ? '0' : '1';
			}
		}
	}
}

char* crc(char* dataword, char* generator) {
	int m = strlen(generator);
	int n = strlen(dataword);
	char dividend[MAX];
	strcpy(dividend, dataword);
	for(int i=0; i<m-1; i++) {
		dividend[n+i]='0';
	}
	dividend[n+m-1]='\0';
	xorDiv(dividend, generator, n+m-1, m);
	char crc[MAX];
	strncpy(crc, &dividend[n], m-1);
	crc[m-1]='\0';
	strcat(dataword, crc);
	return dataword;
}

int main() {
	int server_fd, client_fd, server_len, client_len;
	Word m;
	struct sockaddr_in server_addr, client_addr;
	char result[MAX];
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	client_len = sizeof(client_addr);
	server_len = sizeof(server_addr);
	memset(&m, 0, sizeof(m));
	memset(result, 0, sizeof(result));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(ip);
	
	bind(server_fd, (struct sockaddr*)&server_addr, server_len);
	listen(server_fd, 1);
	client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
	
	while(1) {
	
		int n = read(client_fd, &m, sizeof(m));
		
		printf("The dataword received is %s\n",m.dataword);
		printf("The generator received is: %s\n",m.generator);
		
		if(!strncmp(m.dataword, "exit", 4) || !strncmp(m.generator, "exit", 4)) {
			printf("Server terminated\n");
			break;
		}
		
		strcpy(result, crc(m.dataword, m.generator));
		
		write(client_fd, result, sizeof(result));
		memset(&m, 0, sizeof(m));
		memset(result, 0, sizeof(result));
	}
	close(client_fd);
	close(server_fd);
	
	return 0;
}