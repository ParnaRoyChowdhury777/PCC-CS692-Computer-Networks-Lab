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
	
	while(1) {
	
		printf("Receiving data from client\n");
		if(msgrcv(msgid, (void*)&my_data, sizeof(my_data) - sizeof(my_data.type), 2, 0) == -1) {
			perror("Msgrcv failed");
			exit(0);
		}
		printf("Received data from client: %d\n", my_data.roll);
		
		if(my_data.roll == -1) {
			printf("Terminated\n");
			break;
		}
		
		char str[32];
		sprintf(str, "%d", my_data.roll);
		
		printf("Printed in reverse order: \n");
		
		for(int i=strlen(str); i>=0; i--) {
			printf("%c",str[i]);
		}
		printf("\n");
	}
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}


