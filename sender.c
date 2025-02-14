#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>

#include<sys/msg.h>

#define MAX 100

typedef struct {
	long type;
	char num[MAX];
}my_msg;

void reverse(char* str) {
	int len = strlen(str);
	for(int i=0; i<len/2; i++) {
		char temp = str[i];
		str[i] = str[len - i -1];
		str[len - i - 1] = temp;
	}
}

void decimalToBinary(int n, char* dec) {
	int i = 0;
	while(n > 0) {
		dec[i] = (n % 2) + '0';
		n/=2;
		i++;
	}
	dec[i] = '\0';
	reverse(dec);
}

void decimalToOctal(int n, char* oct) {
	int i = 0;
	while(n > 0) {
		oct[i] = (n % 8) + '0';
		n/=8;
		i++;
	}
	oct[i] = '\0';
	reverse(oct);
}


void decimalToHex(int n, char* hex) {
	int i = 0;
	while(n > 0) {
		hex[i] = (n % 16) < 10 ? (n % 16) + '0' : (n % 16) - 10 + 'A' ;
		n/=16;
		i++;
	}
	hex[i] = '\0';
	reverse(hex);
}

int main() {
	my_msg bin_msg, oct_msg, hex_msg;
	char bin[BUFSIZ], oct[BUFSIZ], hex[BUFSIZ];

	int msgid = msgget(1274, 0666 | IPC_CREAT);

	if(msgid == -1) {
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	printf("This is the sender process\n");
	
	while(1) {
		int n;
		printf("Enter the number(press -1 to terminate): \n");
		scanf("%d",&n);

		bin_msg.type = 1;
		oct_msg.type = 2;
		hex_msg.type = 3;
		
		if(n == -1) {
			strcpy(bin_msg.num, "end\n");
			strcpy(oct_msg.num, "end\n");
			strcpy(hex_msg.num, "end\n");
			msgsnd(msgid, (void*)&bin_msg, MAX, 0);
			msgsnd(msgid, (void*)&oct_msg, MAX, 0);
			msgsnd(msgid, (void*)&hex_msg, MAX, 0);
			printf("Program terminated\n");
			break;
		}

		decimalToBinary(n, bin);
		decimalToOctal(n, oct);
		decimalToHex(n, hex);

		strcpy(bin_msg.num, bin);
		strcpy(oct_msg.num, oct);
		strcpy(hex_msg.num, hex);

		if(msgsnd(msgid, (void*)&bin_msg, MAX, 0) == -1) {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}

		if(msgsnd(msgid, (void*)&oct_msg, MAX, 0) == -1) {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}

		if(msgsnd(msgid, (void*)&hex_msg, MAX, 0) == -1) {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}

	}
	return 0;
}
