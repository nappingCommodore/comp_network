#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>

#define SIZE 1024

void e_error(char* msg){
	printf("%s\n",msg);
	exit(0);
}

int main(int argc,char* argv[]){

	int port = atoi(argv[1]);
	int sfd,nsfd;
	struct sockaddr_in addr;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		e_error("socket() error");
	}

	bzero((struct sockaddr_in*)&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(port);

	int x = bind(sfd,(struct sockaddr*)&addr,sizeof(addr));
	if(x<0){
		e_error("bind() error in pl");
	}

	x = listen(sfd,10);
	if(x<0){
		e_error("listen() error");
	}

	while(1){
		int len = sizeof(addr);
		nsfd = accept(sfd,(struct sockaddr*)&addr,&len);
		char msg[SIZE];
		read(nsfd,msg,SIZE);

		printf("%s\n",msg);

		read(nsfd,msg,SIZE);
		if(strcasecmp(msg,"exit") == 0){
			printf("Train exiting from platform %d..\n",port);
			if(port == 9095)
				kill(getppid(),SIGUSR1);
			else
			if(port == 9096)
				kill(getppid(),SIGUSR2);
			else
			if(port == 9097)
				kill(getppid(),SIGQUIT);
		}
	}
}
