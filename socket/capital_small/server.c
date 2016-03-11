#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define SIZE 1024

void error(const char* msg){
	perror(msg);
	exit(1);
}

int main(){
	int sfd,nsfd;
	struct sockaddr_in servaddr;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		error("Socket error");
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(8081);

	int x = bind(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
	if(x < 0){
		error("Bind error");
	}

	x = listen(sfd,10);
	if(x < 0){
		error("listening error");
	}

	int len = sizeof(servaddr);
	nsfd = accept(sfd,(struct sockaddr*)&servaddr,&len);
	if(nsfd < 0){
		error("Accept error");
	}

	char msg[SIZE];

	while(1){
		bzero(msg,SIZE);
		read(nsfd,msg,SIZE);
		int i;

		if(strcasecmp(msg,"exit\n") == 0){
			printf("Exiting .....\n");
			exit(0);
		}

		for(i=0;msg[i]!='\0';i++){
			msg[i] = toupper(msg[i]);
		}

		printf("message in upper case:- %s\n",msg);
		printf("\nEnter your message : ");
		fgets(msg,SIZE,stdin);
		write(nsfd,msg,SIZE);
		if(strcasecmp(msg,"exit\n") == 0){
			printf("Exiting .....\n");
			exit(0);
		}
	}
}