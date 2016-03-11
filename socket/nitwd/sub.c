#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char*argv[]){
	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	printf("a:%d b:%d\n",a,b);

	int res = a-b;
	int fifo_fd = open("subfifo",O_RDWR);

	printf("res (sub): %d\n",res);
	write(fifo_fd,&res,10);

	close(fifo_fd);
}