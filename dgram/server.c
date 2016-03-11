#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <error.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024


void _error(char* msg){
	perror(msg);
	exit(0);
}

int main(int argc,char* argv[]){
	if(argc != 2){
		_error("Format : ./a.out <port>");
	}

	char msg[SIZE];
	int sfd,port,c_len;
	struct sockaddr_in s_addr,c_addr;

	port = atoi(argv[1]);

	c_len = sizeof(c_addr);

	sfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(sfd < 0){
		_error("socket() error.");
	}

	memset((char*)&s_addr,0,sizeof(s_addr));

	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(port);

	int x = bind(sfd,(struct sockaddr*)&s_addr,sizeof(s_addr));
	if(x < 0){
		_error("bind() error.");
	}

	while(1){
		printf("waiting for message...\n");
		memset(msg,'\0', SIZE);
		int recv;
		fflush(stdout);
		if((recv = recvfrom(sfd,msg,SIZE,0,(struct sockaddr*)&c_addr,&c_len)) < 0){
			_error("recvfrom() error.");
		}
		printf("%s msg is received from client %s : %d\n",msg,inet_ntoa(c_addr.sin_addr),ntohs(c_addr.sin_port));
	}

	return 0;
}