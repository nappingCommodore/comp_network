#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <malloc.h>
#include <fcntl.h>

#define MAX_SIZE 1024

int main(){
	char* myfifo = "/home/niks/my_codes/comp_network/multiserver/myfifo";
	int i;
	mkfifo(myfifo,0777);
	int fd = open(myfifo,O_WRONLY);
	char* msg = (char*) malloc(sizeof(char)*MAX_SIZE);
	printf("Enter your msg.\n");
	scanf("%s",msg);
	write(fd,msg,MAX_SIZE);
	return 0;
}

