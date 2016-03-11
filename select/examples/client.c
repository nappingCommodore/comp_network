#include <stdio.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SIZE 1024


int main(){
	int sfd;
	struct sockaddr_in addr;

	bzero((struct sockaddr_in*)&addr,sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(8080);

	sfd = socket(AF_INET,SOCK_STREAM,0);

	connect(sfd,(struct sockaddr*)&addr,sizeof(addr));

	while(1){
		char msg[SIZE];
		printf("Enter msg..\n");
		scanf("%s",msg);
		send(sfd,msg,SIZE,0);
	}
}
