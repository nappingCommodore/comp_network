#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <malloc.h>
#include <error.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>

#define MAX_SIZE 1024

int *ch;
int shmid;

int main(){
	shmid = shmget(123,MAX_SIZE,0777|IPC_CREAT);
	ch = shmat(shmid,NULL,0);
	char myfifo[MAX_SIZE] = "/home/niks/my_codes/comp_network/chat_server/myfifo1";
	char msg[MAX_SIZE];
	int fd;
	while(1){
		if(*ch == 1){
			fd = open(myfifo,O_RDWR);
			printf("Enter your msg..\n");
			scanf("%s",msg);
			write(fd,msg,MAX_SIZE);
			read(fd,msg,MAX_SIZE);
			printf("%s",msg);
			close(fd);
		}
		
		else{
			sleep(5);
			fd = open(myfifo,O_RDWR);
			read(fd,msg,MAX_SIZE);
			printf("client 1 received a message : %s",msg);
			close(fd);
		}
		
		
	}
	
	return 0;
}

