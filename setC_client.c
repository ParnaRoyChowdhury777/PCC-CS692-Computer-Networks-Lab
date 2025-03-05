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

int main() {
	my_msg data;
	int msgid = msgget(1698, 0666 | IPC_CREAT);
	if(msgid == -1) {
		perror("Msgget failed");
		exit(0);
	}
	while(1) {
		printf("Enter a bit stream of 2 bytes: \n");
		fgets(data.stream, MAX, stdin);
		data.type = 2;
		
		printf("Sending bit-stream %s to the server\n", data.stream);
		if(msgsnd(msgid, (void*)&data, MAX, 0) == -1) {
			perror("Msgsnd failed");
			exit(0);
		}
		printf("Sent bit-stream %s to the server\n", data.stream);
		
		if(!strcmp(data.stream, "-1\n")) {
			printf("Program terminated\n");
			break;
		}
		
		printf("Receiving bit-stream from the server\n");
		if(msgrcv(msgid, (void*)&data, MAX, 1, 0) == -1) {
			perror("Msgrcv failed");
			exit(0);
		}
		printf("Modified bit-stream %s received from the server\n", data.stream);
	}
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
