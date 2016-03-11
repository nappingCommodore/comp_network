#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>

#define SIZE 1024

void error(const char *msg){
	perror(msg);
	exit(0);
}

int main(){
	int sfd,nsfd;
	struct sockaddr_in servaddr;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		error("socket error : ");
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);//"127.0.0.1"
	servaddr.sin_port = htons(8080);


	int x = bind(sfd,(struct sockaddr*) &servaddr,sizeof(servaddr));
	if(x < 0){
		error("bind error : ");
	}
 

	x = listen(sfd,10);
	if(x < 0){
		error("listen error : ");
	}

	while(1){
		int len = sizeof(servaddr);
		nsfd = accept(sfd,(struct sockaddr*) &servaddr,&len);
		if(nsfd < 0){
			error("accept error : ");
		}

		char msg[SIZE];
		read(nsfd,msg,SIZE);

		printf("Msg recieved %s\n",msg);

	}
}