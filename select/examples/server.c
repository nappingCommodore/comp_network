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
	int sfd,nsfd;
	fd_set readfds,writefds,tmp;
	struct sockaddr_in servaddr;
	int max_fd;
	struct timeval timeout;

	bzero((struct sockaddr*)&servaddr,sizeof(servaddr));
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	sfd = socket(AF_INET,SOCK_STREAM,0);

	FD_SET(sfd,&readfds);
	FD_SET(sfd,&writefds);

	max_fd = sfd;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr.sin_port = htons(8080);

	bind(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	listen(sfd,2);

	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	while(1){
		int len = sizeof(servaddr);
		tmp = readfds;
		int x = select(max_fd+1,&readfds,NULL,NULL,&timeout);

		if(x>0){
			int i;

			for(i=0;i<=max_fd;i++){
				printf("i = %d\n",i);
				printf("sfd = %d\n",sfd);
				if(FD_ISSET(i,&readfds)){
					if(i == sfd){
						nsfd = accept(sfd,(struct sockaddr*)&servaddr,&len);
						FD_SET(nsfd,&readfds);
						if(nsfd > max_fd)
							max_fd = nsfd;
						printf("\nNew client got session....\n");
						//close(sfd);
					}

					else{
						char msg[SIZE];
						recv(i,msg,SIZE,0);
						printf("\nmessage received from client is.. %s\n",msg);
					}
				}
			}
		}
	}
}

