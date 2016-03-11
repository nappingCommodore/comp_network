#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <error.h>
#include <string.h>

#define SIZE 1024


void _error(char* msg){
	perror(msg);
	exit(0);
}

int main(int argc,char* argv[]){
	if(argc != 2){
		_error("Format : ./a.out <port>");
	}

	int port = atoi(argv[1]);

	int sfd;
	struct sockaddr_in s_addr;

	sfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sfd < 0){
		_error("socket() error.");
	}

	memset((char*)&s_addr,0,sizeof(s_addr));
	s_addr.sin_family = AF_INET;
	s_addr.sin_port = htons(port);

	if(inet_aton("127.0.0.1",&s_addr.sin_addr) == 0)
		_error("inet_aton() error.");


	int len = sizeof(s_addr);
	char msg[SIZE];
	int i=0;

	while(1){
		sleep(2);
		printf("sending message....\n");
		sprintf(msg,"This is message no. %d\n",i++);
		printf("%s\n",msg);
		if(sendto(sfd,msg,SIZE,0,(struct sockaddr*)&s_addr,len) < 0){
			_error("sendto() error.");
		}
		sleep(2);
		printf("message sent...\n");
	}

	return 0;
}