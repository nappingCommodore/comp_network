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

int no_of_clients;
int shmid;
fd_set readfds,writefds,exceptfds;
struct timeval timeout;

struct shared_mem{
	bool flag;
	int sender;
	int receiver;
	int clients;
};

struct shared_mem *shm;

void Error(char *msg){
	perror("Error ");
	printf("%s \n",msg);
	exit(0);
}

void init(){
	if((shmid = shmget(1234,SIZE,0666|IPC_CREAT)) < 0){
		Error("shmget");
	}
	
	shm = (struct shared_mem*)shmat(shmid,NULL,0);
	
	if(shm == (struct shared_mem*)-1){
		Error("shmat");
	}
	
	shm->flag = false;
	shm->sender = -1;
	shm->receiver = -1;
	shm->clients = no_of_clients;
	
	int i;
	for(i=0;i<no_of_clients;i++){
		char myfifo[SIZE];
		snprintf(myfifo,SIZE,"myfifo%d",i);
		if(mkfifo(myfifo,0666) < 0){
			Error("mkfifo");
		}
		
		else{
			printf("myfifo%d created\n",i);
		}
	}
}


void prep_select(int fd[],int no_of_clients){
	int i;
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	for(i=0;i<no_of_clients;i++){
		char myfifo[SIZE];
		snprintf(myfifo,SIZE,"myfifo%d",i);
		fd[i] = open(myfifo,O_RDWR);
		FD_SET(fd[i],&readfds);
		FD_SET(fd[i],&writefds);
	}
	
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
}


void unlink_fifos(){
	int i;
	for(i=0;i<shm->clients;i++){
		char myfifo[SIZE];
		snprintf(myfifo,SIZE,"myfifo%d",i);
		unlink(myfifo);
		printf("myfifo%d has been deleted..\n",i);
	}
}

int main(int argc,char**argv){
	
	if(argc != 2){
		perror("Format : ./a.out <number of clients>\n");
		exit(0);
	}
	
	no_of_clients = atoi(argv[1]);
	int fd[no_of_clients];
	
	int send,recv;
	
	init();	//shm and fifos have been created...
	
	prep_select(fd,no_of_clients);

	int ret = select(FD_SETSIZE,&readfds,&writefds,NULL,&timeout);	
		
	if(ret > 0){
		while(1){
			printf("Enter sender and receiver...");
			scanf("%d %d",&send,&recv);
			
			if(send == -1 && recv == -1)
				break;
			
			if(send >= no_of_clients || recv >= no_of_clients){
				perror("Error: ");
				printf("Value should be between 0 to %d\n",no_of_clients-1);
				continue;
			}
				shm->flag = true;
				shm->sender = send;
				shm->receiver = recv;
		}
	}
	
	unlink_fifos();
	
	return 0;
}

