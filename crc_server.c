#include "custom.h"

/*char* crc(char* generator, char* dataword) {
    int i,j,m=strlen(generator),n=strlen(dataword);
    char remainder[50];
    for(i=0;i<m;i++) remainder[i]=dataword[i];
    for(i=m;i<=n;i++) {
        if(remainder[0]=='1') {
            for(j=1;j<m;j++) {
                remainder[j]=remainder[j]==generator[j]?'0':'1';
            }
        }
        for(j=0;j<m-1;j++) remainder[j]=remainder[j+1];
        remainder[j]=dataword[i];
    }
    remainder[m]='\0';
    char* res=(char*)malloc(50*sizeof(char));
    j=0;
    for(i=0;i<n;i++) {
        if(i<=n-m) res[i]=dataword[i];
        else res[i]=remainder[j++];
    }
    res[i]='\0';
    return res;
}*/


void xorDiv(char *divisor, char* dividend, int n, int m) {
	for(int i=0; i<=n-m; i++) {
		if(dividend[i] == '1') {
			for(int j=0; j<m; j++) {
				dividend[i + j] = (dividend[i+j] == divisor[j]) ? '0' : '1';
			}
		}
	}
}

char *crc(char *generator, char* dataword) {
	int m = strlen(generator);
	int n = strlen(dataword);
	
	char dividend[BUFF_SIZE];
	strcpy(dividend, dataword);
	
	for(int i=0; i<m-1; i++) {
		dividend[n+i] = '0';
	}
	dividend[n +m -1] = '\0';
	
	xorDiv(generator, dividend, n + m - 1, m);
	
	char crc[BUFF_SIZE];
	
	strncpy(crc, &dividend[n], m-1);
	crc[m-1] = '\0';
	strcat(dataword, crc);
	return dataword;
	
}

int main() {
    struct sockaddr_un server_addr, client_addr;
    int server_len, client_len;
    Word buff;
    unlink(SOCKET_PATH);
    
    int server_fd=socket(AF_UNIX,SOCK_STREAM,0);
    if (server_fd < 0) {
		perror("SERVER: Failed to create socket\n");
		exit(0);
    }
    printf("Server Socket created\n");
    
    server_len = sizeof(server_addr);
    memset(&server_addr,0,server_len);
    server_addr.sun_family=AF_UNIX;
    strcpy(server_addr.sun_path,SOCKET_PATH);
    
    
    if(bind(server_fd, (struct sockaddr*)&server_addr, server_len) < 0) {
		perror("SERVER: Bind failed\n");
		exit(0);
    }
    
    if(listen(server_fd, 20) < 0) {
		perror("SERVER: Listen failed\n");
		exit(0);
    }
    
    while(true) {
        printf("Server waiting\n");
	client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if(client_fd < 0) {
		perror("SERVER: Accept failed\n");
		exit(0);
	}
	
        if(read(client_fd,&buff,sizeof(buff)) < 0) {
        	perror("SERVER: Read failed\n");
		exit(0);
        }
        
        char res[BUFF_SIZE];
        strcpy(res,crc(buff.gen,buff.dw));
        printf("Transmitting Codeword to client...\n");
        if(write(client_fd,res,sizeof(res)) < 0) {
        	perror("SERVER: Write failed\n");
		exit(0);
        }
        
        memset(res,0,sizeof(res));
        close(client_fd);
    }
    close(server_fd);
    unlink(SOCKET_PATH);
    return 0;
}
