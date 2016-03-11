#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <sys/shm.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

#define SIZE 1024

int opid;

struct val{
	bool flag;
	int op;
	int a,b;
	int res;
};

struct val* v;

void* operation(void* args){
	int op;
	printf("Enter number of operation.\n");
	scanf("%d",&v->op);
	printf("Enter value of a and b.\n");
	scanf("%d %d",&v->a,&v->b);
	v->flag = true;
	
	sleep(5);
	int a,b,res;
	mkfifo("resfifo",0666);
	int fd = open("resfifo",O_RDWR);
	write(fd,&a,SIZE);
	write(fd,&b,SIZE);
	read(fd,&res,SIZE);
	close(fd);
	
	printf("res is : %d\n",v->res);
}

int main(){
	pthread_t thread[3];
	
	opid = shmget(3456,SIZE,0666|IPC_CREAT);
	v = (struct val*) shmat(opid,NULL,0);
	
	int i;
	
	for(i=0;i<3;i++){
		pthread_create(&thread[i],NULL,operation,NULL);
	}
	
	for(i=0;i<3;i++){
		pthread_join(thread[i],NULL);
	}

}
