#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/times.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <stdbool.h>

#define SIZE 1024

int opid;

struct val{
	int op;
	int a,b;
	int res;
};

struct val* v;

int main(){
	
	opid = shmget(3456,SIZE,0666|IPC_CREAT);
	v = (struct val*) shmat(opid,NULL,0);
	
	int a,b,res;
	char add[SIZE];
	//mkfifo("resfifo",0666);
	int fd = open("resfifo",O_RDWR);
	read(fd,&a,SIZE);
	read(fd,&b,SIZE);
	printf("%d %d\n",a,b);
	v->res = a+b;
	res = a+b;
	write(fd,&res,SIZE);
	printf("Result is: %d\n",res);
	close(fd);
}
