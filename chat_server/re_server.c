#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <malloc.h>
#include <error.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <time.h>

#define MAX_SIZE 1024

struct pollfd *fds;
int no_of_fds;
int shmid;
int *ch;	//for shared memory

void create_shm(){
	shmid = shmget(123,MAX_SIZE,0777|IPC_CREAT);
	ch = shmat(shmid,NULL,0);
}

void create_fifos(){
	char myfifo[MAX_SIZE];
	int i,x;
	for(i=0;i<no_of_fds;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
		if((x = mkfifo(myfifo,0777)) != -1){
			printf("myfifo%d created ...\n",i);
		}
	}
}


void unlink_fifos(){
	char myfifo[MAX_SIZE];
	int i,x;
	for(i=0;i<no_of_fds;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
		if((x = unlink(myfifo)) != -1){
			printf("myfifo%d destroyed ...\n",i);
		}
	}
}

void open_fds(){
	char myfifo[MAX_SIZE];
	int i,x;
	for(i=0;i<no_of_fds;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
		if((fds[i].fd = open(myfifo,O_RDWR))>0){
			printf("fds[%d].fd is open..\n",i);
		}
		fds[i].events = POLLIN | POLLOUT;
		
	}	
}

void close_fds(){
	int i;
	for(i=0;i<no_of_fds;i++){
		if(close(fds[i].fd) == 0){
			printf("fds[%d].fd is close..\n",i);
		}
	}
	
}

int main(int argc,char**argv){
	
	if(argc!=2){
		perror("Format : ./a.out <no_of_fds>");
		exit(0);
	}
	
	int i;
	no_of_fds = atoi(argv[1]);
	
	fds = malloc(sizeof(struct pollfd)*no_of_fds);
	
	create_shm();
	
	create_fifos();
	printf("\n");
	open_fds();
	printf("\n");
	
	int timeout_msecs = 500;
	int ret = poll(fds,no_of_fds,timeout_msecs);
	char msg[MAX_SIZE];
	char myfifo[MAX_SIZE];
	int count = 0;
	
	if(ret > 0){
		while(1){
			/*for(i=0;i<no_of_fds;i++){
				if(fds[i].revents & POLLOUT){
					strcpy(msg,"Please write your msg.");
					write(fds[i].fd,msg,MAX_SIZE);
					sleep(5);
					read(fds[i].fd,msg,MAX_SIZE);
					int j;
					for(j=0;j<no_of_fds;j++){
						if(i!=j){
							write(fds[i].fd,msg,MAX_SIZE);
						}
					}
				}
			}*/
			/*srand(time(NULL));
			int a = ((double)rand() / (double)((unsigned)RAND_MAX + 1))*no_of_fds;
			printf("a = %d\n",a);
			*ch = a;*/
			//printf("rand %d\n",*ch);
			
			for(i=0;i<no_of_fds;i++){
				if(fds[i].revents & POLLIN){
					*ch = i;
					read(fds[*ch].fd,msg,MAX_SIZE);
					printf("*ch = %d , msg = %s\n",*ch,msg);
					int j;
					for(j=0;j<no_of_fds;j++){
						if(j!=*ch){
							write(fds[j].fd,msg,MAX_SIZE);
						}
					}
				}
				else{
					int j;
					for(j=0;j<no_of_fds;j++){
						if(j!=*ch){
							write(fds[j].fd,msg,MAX_SIZE);
						}
					}
				}
			sleep(5);	
			}
			
		}
	}
	
	close_fds();
	printf("\n");
	unlink_fifos();
	printf("\n");
}
