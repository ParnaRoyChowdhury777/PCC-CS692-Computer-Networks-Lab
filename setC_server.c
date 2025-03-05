#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/msg.h>

#define MAX 32

typedef struct {
	long type;
	char stream[MAX];
}my_msg;

void modify(char *input, char* output) {
	int j=0, count = 0;
	for(int i=0; input[i]!='\0'; i++) {
		output[j++] = input[i];
		if(input[i] == '0') {
			count++;
			if(count == 5) {
				output[j++] = '1';
				count = 0;
			}
		} else {
			count = 0;
		}
	}
	output[j] = '\0';
}

/*void modify(char *input, char* output) {
	int j=0, count = 0;
	for(int i=0; input[i]!='\0'; i++) {
		output[j++] = input[i];
		if(input[i] == '0') {
			count++;
			if(count == 5 && input[i+1] == '1') {
				i++;
				count = 0;
			}
		} else {
			count = 0;
		}
	}
	output[j] = '\0';
}*/

int main() {
	my_msg data;
	int msgid = msgget(1698, 0666 | IPC_CREAT);
	if(msgid == -1) {
		perror("Msgget failed");
		exit(0);
	}
	
	while(1) {
		printf("Receiving bit-stream from the client\n");
		if(msgrcv(msgid, (void*)&data, MAX, 2, 0) == -1) {
			perror("Msgrcv failed");
			exit(0);
		}
		printf("Received bit-stream from the server: %s\n", data.stream);
		
		if(!strcmp(data.stream, "-1\n")) {
			printf("Program terminated\n");
			break;
		}
		
		char modified_stream[MAX];
		
		
		modify(data.stream, modified_stream);
		
		data.type = 1;
		strcpy(data.stream, modified_stream);
		
		printf("Sending modified bit-stream %s to the server\n", data.stream);
		if(msgsnd(msgid, (void*)&data, MAX, 0) == -1) {
			perror("Msgsnd failed");
			exit(0);
		}
		printf("Sent modified bit-stream %s to the server\n", data.stream);
	}
	return 0;
}
