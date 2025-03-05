#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/msg.h>

typedef struct{
	long type;
	int roll;
}data;

int main() {
	int msgid = msgget(1567, 0666 | IPC_CREAT);
	if(msgid == -1) {
		perror("Msgget failed");
		exit(0);
	}
	data my_data;
	int n;
	while(1) {
		printf("Enter your roll no(-1 to terminate): \n");
		scanf("%d",&n);
		
		my_data.type = 2;
		my_data.roll = n;
		
		printf("Sending data to server\n");
		if(msgsnd(msgid, (void*)&my_data, sizeof(my_data) - sizeof(my_data.type), 0) == -1) {
			perror("Msgsnd failed");
			exit(0);
		}
		printf("Sent data to server\n");
		if(n == -1) {
			printf("Terminated\n");
			break;
		}
	}
	return 0;
}
