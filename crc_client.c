#include "custom.h"

int main() {
    Word m;
    char res[BUFF_SIZE];
    struct sockaddr_un client_addr;
    
    int client_fd=socket(AF_UNIX,SOCK_STREAM,0);
    if (client_fd < 0) {
		perror("CLIENT: Failed to create socket\n");
		exit(0);
    }
    printf("Client Socket created\n");
    
    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sun_family=AF_UNIX;
    strcpy(client_addr.sun_path,SOCKET_PATH);
    
    int client_len = sizeof(client_addr);
    
    if(connect(client_fd, (struct sockaddr*)&client_addr, client_len) < 0) {
		perror("CLIENT: Connect failed\n");
		exit(0);
    }
    
    printf("CLIENT: Enter the generator: ");
    scanf(" %[^\n]s",m.gen);
    printf("CLIENT: Enter the dataword: ");
    scanf(" %[^\n]s",m.dw);
    
    if(write(client_fd,&m,sizeof(m)) < 0) {
    	perror("CLIENT: Write failed\n");
	exit(0);
    }
    
    if(read(client_fd,res,sizeof(res)) < 0) {
    	perror("CLIENT: Read failed\n");
	exit(0);
    }
    
    printf("Codeword Obtained from server: %s\n",res);
    close(client_fd);
    return 0;
}
