#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <malloc.h>
#include <sys/shm.h>
#include <fcntl.h>

#define MAX_SIZE 1024

struct pollfd* fds;
int no_of_clients;
int shmid_pid,shmid_flg;
int* pid;
int *int_buf;
//"/home/niks/my_codes/comp_network/chat/myfifo"

void shm_create(){
	key_t key = 123;
	shmid_pid = shmget(key,MAX_SIZE,IPC_CREAT | 0777);
	pid = (int*) shmat(shmid_pid,NULL,0);
	
	key = 456;
	shmid_flg = shmget(key,MAX_SIZE,IPC_CREAT | 0777);
	int_buf = (int*) shmat(shmid_flg,NULL,0);
}

void create_fifos(){
	char* myfifo = (char*) malloc(sizeof(char)*MAX_SIZE);
	int i;
	for(i=0;i<no_of_clients;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat/myfifo%d",i);
		mkfifo(myfifo,0777);
		printf("fifo myfifo%d created.",i);
	}
}

void unlink_fifos(){
	char *myfifo = (char*) malloc(sizeof(char)*MAX_SIZE);
	int i;
	for(i=0;i<no_of_clients;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat/myfifo%d",i);
		unlink(myfifo);
		printf("fifo myfifo%d destroyed.",i);
	}
}


void open_fds(){
	char* myfifo = (char*) malloc(sizeof(char)*MAX_SIZE);
	int i;
	for(i=0;i<no_of_clients;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat/myfifo%d",i);
		fds[i].fd = open(myfifo,O_RDWR);
		fds[i].events = POLLIN|POLLOUT;
		printf("fd%d opened.",i);
	}
}


void close_fds(){
	char* myfifo = (char*) malloc(sizeof(char)*MAX_SIZE);
	int i;
	for(i=0;i<no_of_clients;i++){
		close(fds[i].fd);
		printf("fd%d closed.",i);
	}
}


int main(int argc,char**argv){
	if(argc != 2){
		perror("Format : ./a.out <number of clients>");
		exit(0);
	}
	
	shm_create();
	
	no_of_clients = atoi(argv[1]);
	int i;
	
	fds = (struct pollfd*) (malloc(sizeof(struct pollfd)*no_of_clients));
	create_fifos();
	
	open_fds();	//all fds are open now with read write permission
	int time_msecs = 500;
	
	int ret = poll(fds,no_of_clients,time_msecs);
	char* msg = (char*) malloc(sizeof(char)*MAX_SIZE);
	if(ret > 0){
		while(1){
			for(i = 0;i<no_of_clients;i++){
				if(fds[i].revents & POLLIN){
					*pid = i;
					*int_buf = POLLIN;
				}
				else{
					*pid = i;
					*int_buf = POLLOUT;
				}
			}
		}
	}
	
	close_fds();
	
	unlink_fifos();
	return 0;
}
