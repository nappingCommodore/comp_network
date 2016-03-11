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

struct val{
	int op;
	int a,b;
	int res;
};

int opid;

struct val* v;

int main(){
	
	opid = shmget(3456,SIZE,0666|IPC_CREAT);
	v = (struct val*) shmat(opid,NULL,0);
	
	int a,b,res;
	int fd = open("resfifo",O_RDWR);
	read(fd,&a,SIZE);
	read(fd,&b,SIZE);
	v->res = a*b;
	res = a*b;
	write(fd,&res,SIZE);
	printf("Result is: %d\n",res);
	close(fd);
}
