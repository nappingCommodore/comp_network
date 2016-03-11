//poll reads complete file in one go (I think so)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>

#define MAX_SIZE 1024
#define NO_OF_FILES 3

void create_fifos(int no_of_files){
	int i;
	char myfifo[MAX_SIZE];
	for(i=0;i<no_of_files;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/poll/myfifo%d",i);
		mkfifo(myfifo,0777);
	}
}


void unlink_fifos(int no_of_files){
	int i;
	char myfifo[MAX_SIZE];
	for(i=0;i<no_of_files;i++){
		snprintf(myfifo,MAX_SIZE,"/home/niks/my_codes/comp_network/poll/myfifo%d",i);
		unlink(myfifo);
	}
}

int main(){
	struct pollfd fds[NO_OF_FILES];
	int i;
	
	for(i=0;i<NO_OF_FILES;i++)
		fds[i].fd = -1;
	
	char file_path[MAX_SIZE];
	int timeout_msecs = 500;
	
	//create_fifos(NO_OF_FILES);
	
	for(i=0;i<NO_OF_FILES;i++){
		snprintf(file_path,MAX_SIZE,"/home/niks/my_codes/comp_network/poll/file%d",i);
		fds[i].fd = open(file_path,O_RDONLY);
		fds[i].events = POLLRDNORM | POLLIN;
	}
	
	int j;
	char buffer[MAX_SIZE];
	int ret = poll(fds,NO_OF_FILES,timeout_msecs);
	if(ret>0){
		while(1){
			for(i=0;i<NO_OF_FILES;i++){
				//printf("i:%d j:%d",i,j);
				if(fds[i].revents & POLLIN){	//if(fds[i].revents == POLLIN)
					read(fds[i].fd,buffer,MAX_SIZE);
					printf("from client%d we read (POLLRDNORM) : %s\n\n",i,buffer);
					/*read(fds[i].fd,a,MAX_SIZE); //data1->a
					read(fds[i].fd,b,MAX_SIZE);
					int pd = fork();
					if(pd == 0){
						
						execl("sum","-c",NULL);
					}*/
				}
			
			}
			sleep(5);
		}
	}
	return 0;
}
