#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include<sys/msg.h>

typedef struct {
	long type;
	char num[BUFSIZ];
}my_msg;

int main() {
	my_msg bin_msg;
	int msgid = msgget(1274, 0666 | IPC_CREAT);
	long received = 1;
	if(msgid == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(1) {
		if(msgrcv(msgid, (void*)&bin_msg, BUFSIZ, received, 0) == -1) {
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s\n", bin_msg.num);
		if (strcmp(bin_msg.num, "end\n") == 0) {
			printf("Program terminated\n");
			break;
		}
	}
	msgctl(msgid, IPC_RMID, 0);
	return 0;
}
