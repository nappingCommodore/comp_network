#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <fcntl.h>

#define SIZE 1024

void error(const char* msg){
	perror(msg);
	exit(0);
}

int main(){
	int sfd;
	struct sockaddr_in servaddr;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		error("socket message : ");
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);

	inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));


	int x = connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(x < 0){
		error("Connect error : ");
	}

	int choice;
	while(1){	
	P:

	printf("Enter your choice of service.\n1.addition\n2.subtraction\n3.multiplication\n");
	scanf("%d",&choice);
	//printf("%d\n",choice);
	if(choice < 1 || choice > 3){
		printf("Requested service is not available.\n");
		goto P;
	}

	char myfifo[SIZE];
	snprintf(myfifo,SIZE,"myfifo%d",choice);
	if(mkfifo(myfifo,0666) < 0)
		error("fifo creation error : ");
	printf("fifo %s created\n",myfifo);
	//mkfifo(myfifo,0666);

	int fifo_fd = open(myfifo,O_RDWR);
	if(fifo_fd < 0){
		error("file open error : ");
	}

	printf("fd %d opened.\n",fifo_fd);

	
		char msg[SIZE];

		int a,b;

		int pid = fork();
		if(pid < 0){
			error("fork error : ");
		}
		printf("After fork\n");
		if(pid == 0){	//child process
			//printf("inside child process.\n");
			switch(choice){
				case 1:	if(execl("sum","c",NULL) < 0) error("exec error : "); break;
				case 2: if(execl("sub","c",NULL) < 0) error("exec error : "); break;
				case 3: if(execl("mul","c",NULL) < 0) error("exec error : "); break;
			}
		}

		else{
			//printf("Inside parent process.\n");
			wait();
			read(fifo_fd,msg,SIZE);
			write(sfd,msg,SIZE);
		}
		close(fifo_fd);
		unlink(myfifo);
	}

	return 0;
}
