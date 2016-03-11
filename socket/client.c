#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>

#define SIZE 1024

int main(){
	int sfd;
	struct sockaddr_in servaddr;

	sfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8080);

	inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));

	connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	char sndmsg[SIZE];
	char rcvmsg[SIZE];

	while(1){
		bzero(sndmsg,SIZE);
		bzero(rcvmsg,SIZE);

		printf("Enter your msg : ");
		fgets(sndmsg,SIZE,stdin);

		write(sfd,sndmsg,SIZE);
		if(strcasecmp(sndmsg,"exit\n") == 0){
			printf("Exiting .....\n");
			exit(0);
		}
		read(sfd,rcvmsg,SIZE);
		printf("Echoed msg is : ");
		printf("%s\n",rcvmsg);
	}
}