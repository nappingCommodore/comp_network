#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <fcntl.h>

#define SIZE 1024

int main(){
	int op1,op2;
	int fd = open("myfifo1",O_WRONLY);
	printf("Enter two numbers to add\n");
	scanf("%d %d",&op1,&op2);

	int res = op1+op2;
	char msg[SIZE];
	snprintf(msg,SIZE,"result of addition of two numbers is : %d",res);

	write(fd,msg,SIZE);
	close(fd);
	return 0;
}