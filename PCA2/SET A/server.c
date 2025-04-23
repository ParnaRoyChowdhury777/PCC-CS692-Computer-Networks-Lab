#include "custom.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int active_clients = 0;

typedef struct {
	int client_no;
	int client_fd;
	int server_fd;
}client_data;

char* bit_stuffing(char* input) {
	char* output = (char*)malloc(MAX*sizeof(char));
	int count = 0, flag = 0, j = 0;
	for(int i=0; i<strlen(input); i++) {
		output[j++] = input[i];
		if(input[i] == '1') {
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
	output[j] = '\0';
	return output;
}

void* pthBody(void *args) {
	client_data* data = (client_data*)args;
	int client_no = data->client_no;
	int client_fd = data->client_fd;
	int server_fd = data->server_fd;
	free(data);
	
	while(1) {
		char buff[MAX];
		int m = read(client_fd, buff, sizeof(buff));
		buff[m]='\0';
		printf("Dataword from client %d is %s\n", client_no, buff);
		if(!strncmp(buff, "exit", 4)) {
			printf("CLIENT THREAD %d Terminated\n", client_no);
			break;
		}
		char modified[MAX];
		strcpy(modified, bit_stuffing(buff));
		write(client_fd, modified, strlen(modified));
	}
	
	close(client_fd);
	
	pthread_mutex_lock(&lock);
	active_clients--;
	if(active_clients == 0) {
		printf("All clients exited, server shutting down\n");
		close(server_fd);
		exit(0);
	}
	pthread_mutex_unlock(&lock);
	
	return NULL;
}

int main() {
	int client_fd, server_fd, server_len, client_len;
	char buff[MAX];
	struct sockaddr_in client_addr, server_addr;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fd < 0) {
		perror("Server socket failed\n");
		exit(0);
	}
	client_len = sizeof(client_addr);
	server_len = sizeof(server_addr);
	memset(buff, 0, sizeof(buff));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(ip);
	
	bind(server_fd, (struct sockaddr*)&server_addr, server_len);
	listen(server_fd, 5);
	
	int client_no = 0;
	
	while(1) {
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
		pthread_t th;
		
		client_data* data = (client_data*)malloc(sizeof(client_data));
		
		data->client_no = client_no;
		data->client_fd = client_fd;
		data->server_fd = server_fd;
		
		pthread_mutex_lock(&lock);
		active_clients++;
		pthread_mutex_unlock(&lock);
		
		pthread_create(&th, NULL, pthBody, data);
		pthread_detach(th);
		
		printf("Thread for client %d created \n",client_no++);
	}
	
	return 0;
}