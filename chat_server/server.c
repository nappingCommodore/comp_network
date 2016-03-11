#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <malloc.h>
#include <math.h>

#define MAX_SIZE 1024

struct pollfd *fds;

void create_fifos(int no_of_fds){
	char myfifo[MAX_SIZE];
	int i,x;
	for(i=0;i<no_of_fds;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
		if((x = mkfifo(myfifo,0777)) != -1){
			printf("fifo myfifo%d generated..\n",i);
		}
	}
}


void unlink_fifos(int no_of_fds){
	int i;
	for(i=0;i<no_of_fds;i++){
		char myfifo[MAX_SIZE];
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
		if(unlink(myfifo) != -1)
			printf("fifo myfifo%d destroyed..\n",i);
	}
}


void open_fds(int no_of_fds){
	int i;
	for(i=0;i<no_of_fds;i++){
		char myfifo[MAX_SIZE];
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
		fds[i].fd = open(myfifo,O_RDWR);
		printf("%d\n",fds[i].fd);
		fds[i].events = POLLIN | POLLOUT;
	}
}


int main(int argc, char** argv){
	
	if(argc!=2){
		perror("Format : ./a.out <NO_OF_CLIENTS>");
		exit(0);
	}
	
	printf("%d %s %s\n",argc,argv[0],argv[1]);
	
	int no_of_fds = atoi(argv[1]);
	create_fifos(no_of_fds);	//fifo created
	fds = malloc(sizeof(struct pollfd)*no_of_fds);
	int i;
	
	open_fds(no_of_fds);
	
	int timeout_msecs = 500;
	int ret = poll(fds,no_of_fds,timeout_msecs);
	if(ret>0){
		while(1){
			for(i=0;i<no_of_fds;i++){
				char myfifo[MAX_SIZE];
				snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/chat_server/myfifo%d",i);
				
				if(fds[i].revents == POLLIN){
					
				}
			}
		}
		
	}
	unlink_fifos(no_of_fds);	//fifo destroyed
	return 0;
}
