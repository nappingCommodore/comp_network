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
	int sfd,port;
	struct sockaddr_in addr;
	bzero((struct sockaddr_in*)&addr,sizeof(addr));

	port = atoi(argv[1]);

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		e_error("socket() error");
	}

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(port);

	int x = connect(sfd,(struct sockaddr*)&addr,sizeof(addr));

	if(x < 0){
		e_error("connect() error");
	}
	printf("Connected..\n");
	read(sfd,&port,SIZE);

	close(sfd);

	sfd = socket(AF_INET,SOCK_STREAM,0);

	addr.sin_port = htons(port);
	x = connect(sfd,(struct sockaddr*)&addr,sizeof(addr));

	if(x<0){
		e_error("connect() error");
	}

	char msg[] = "train reached....";
	write(sfd,msg,SIZE);
	sleep(1);
	write(sfd,"exit",SIZE);
	close(sfd);
}