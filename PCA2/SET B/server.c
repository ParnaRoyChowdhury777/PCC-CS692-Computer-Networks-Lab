#include "custom.h"

int main() {
	int sock_fd;
	char buff[MAX];
	struct sockaddr_in server_addr, client_addr;
	
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0) {
		perror("Client Socket failed\n");
		exit(0);
	}
	memset(buff,0,sizeof(buff));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(PORT);
	server_addr.sin_addr.s_addr=inet_addr(ip);
	
	bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	while(1) {
		printf("Server waiting\n");
		int l = sizeof(client_addr);
		recvfrom(sock_fd, buff, MAX, 0, (struct sockaddr*)&client_addr, &l);
		printf("Data received : %s\n", buff);
		if(!strncmp(buff, "exit", 4)) {
			printf("Server terminated\n");
			break;
		}
		
		int m = strlen(buff), p=0;
		while(pow(2,p)<m+p+1) p++;
		int sz = m+p, i, buff_c=0;
		char finalMsg[sz];
		
		for(i=0; i<sz; i++) {
			if(ceil(log2(sz-i)) == floor(log2(sz-i))) finalMsg[i]='p';
			else finalMsg[i]=buff[buff_c++];
		}
		finalMsg[sz]='\0';
		
		int bitsCalc = 0;
		while(bitsCalc < p) {
			int posi=pow(2,bitsCalc), bitPosi=posi, ones=0;
			while(bitPosi <= sz) {
				int j=0;
				while(j<posi) {
					if(finalMsg[sz-(bitPosi+j)]=='1') ones++;
					j++;
				}
				bitPosi += 2*posi;
			}
			if(ones % 2 == 0) finalMsg[sz-posi]='0';
			else finalMsg[sz-posi]='1';
			bitsCalc++;
		}
		
		sendto(sock_fd, finalMsg, MAX, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
		memset(buff, 0, sizeof(buff));
	}
	return 0;
}