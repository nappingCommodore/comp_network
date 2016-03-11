#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <error.h>
#include <sys/shm.h>
#include <string.h>
#include <stdbool.h>

#define SIZE 1024

int shmid,no_of_clients,opid;
char* op;
fd_set read_fds,write_fds;
struct timeval timeout;

struct val{
	bool flag;
	int op;
	int a,b;
	int res;
};

struct val* v;
fd_set readfds,writefds,exceptfds;

int main(int argc,char**argv){
	if(argc != 2){
		perror("Format: ./a.out <Number of clients>\n");
		exit(0);
	}
	
	shmid = shmget(1234,SIZE,0666|IPC_CREAT);
	op = (char*)shmat(shmid,NULL,0);
	
	opid = shmget(3456,SIZE,0666|IPC_CREAT);
	v = (struct val*) shmat(opid,NULL,0);
	
	v->flag = false;
	
	int i,fd0,fd1,fd2;
	no_of_clients = atoi(argv[1]);
	
	fd0 = fileno(popen("./add","r"));
	fd1 = fileno(popen("./sub","r"));
	fd2 = fileno(popen("./mul","r"));
	
	FD_ZERO(&readfds);
	FD_SET(fd0,&readfds);
	FD_SET(fd1,&readfds);
	FD_SET(fd2,&readfds);
	
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	
	int retval = select(4,&readfds,NULL,NULL,&timeout);
	
	if(retval)
	{
	while(1){
		if(v->flag == true){
			v->flag = false;
			if(v->op == 1){
				//fd0 = fileno(popen("./add","r"));
				sleep(5);
				int res;
				read(fd0,&res,SIZE);
				printf(" res %d\n",res);
			}
			
			else
			if(v->op == 2){
				//fd1 = fileno(popen("./sub","r"));
				int res;
				read(fd1,&res,SIZE);
				printf(" res %d\n",res);
			}
			
			else{
				//fd2 = fileno(popen("./mul","r"));
				int res;
				read(fd2,&res,SIZE);
				printf(" res %d\n",res);
			}
		}
	}
	}
	
	else
	if(retval == 0){
		printf("%d \n",retval);
		printf("Timeout error\n");
	}
return 0;
}
