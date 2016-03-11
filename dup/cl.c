#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 1024

int main(){
	int fd;
	//dup2(0,fd);	//0 : STDIN	scanf will read into fd
	char msg[SIZE];
	//printf("Enter msg in child.\n");
	scanf("%s",msg);
	printf("Program cl says that : \n");
	printf("Recieved msg is %s\n",msg);
	return 0;
}