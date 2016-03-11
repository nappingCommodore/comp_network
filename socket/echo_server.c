#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>

#define SIZE 1024

void error(const char* msg){
	perror(msg);
	exit(1);
}

int main(int argc,char** argv){

	struct sockaddr_in servaddr;
	int sfd,nsfd;

	sfd = socket(AF_INET,SOCK_STREAM,0);	//SOCK_STREAM connection oriented
	if(sfd < 0){
		error("error opening socket");
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);	//ip addr
	servaddr.sin_port = htons(8080);	//port

	int x = bind(sfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
	if(x < 0){
		error("error in bind");
	}

	x = listen(sfd,10);
	if(x < 0){
		error("error in listen");
	}

	int len = sizeof(servaddr);
	nsfd = accept(sfd,(struct sockaddr*) &servaddr,&len);
	//printf("%s \n",(char*)&servaddr);
	printf("%d\n",nsfd);

	if(nsfd < 0){
		error("error in accept");
	}

	char msg[SIZE];

	while(1){
		bzero(msg,SIZE);
		read(nsfd,msg,SIZE);
		printf("Echoing back the msg - %s\n",msg);

		if(strcasecmp(msg,"exit\n") == 0){
			printf("Exiting .....\n");
			exit(0);
		}
		write(nsfd,msg,SIZE);

	}

	return 0;
}
