#include "custom.h"
void main() {
	int sock_fd=socket(AF_INET,SOCK_DGRAM,0);
	struct sockaddr_in server_addr,client_addr;
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr(ip);
	server_addr.sin_port=htons(PORT_NO);
	bind(sock_fd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr_in));
	while(1) {
		char buff[MAX];
		int l=sizeof(server_addr);
		printf("Waiting for message from Client...\n");
		recvfrom(sock_fd,buff,MAX,0,(struct sockaddr*)&client_addr,&l);
		printf("Message from client (Dataword): %s\n", buff);
		if(!strncmp(buff, "exit", 4)) break;
		int m=strlen(buff),p=0;
		while(pow(2,p)<p+m+1) p++;
		int sz=m+p,i,buff_c=0;
		char finalMsg[sz];
		for(i=0;i<sz;i++) {
			if(ceil(log2(sz-i))==floor(log2(sz-i))) finalMsg[i]='p';
			else finalMsg[i]=buff[buff_c++];
		}
		finalMsg[sz] = '\0';
		int bitsCalc=0;
		while(bitsCalc<p){
			int posi=pow(2,bitsCalc),bitPosi=posi,ones=0;
			while(bitPosi<=sz){
				int j=0;
				while(j<posi) {
					if(finalMsg[sz-(bitPosi+j)]=='1') ones++;
					j++;
				}
				bitPosi+=2*posi;
			}
			if((ones%2)==0) finalMsg[sz-posi]='0';
			else finalMsg[sz-posi]='1';
			bitsCalc++;
		}
		sendto(sock_fd,finalMsg,MAX,0,(struct sockaddr*)&client_addr,l);
	}
}
