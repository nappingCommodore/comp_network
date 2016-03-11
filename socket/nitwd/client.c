#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <time.h>
#include <sys/select.h>
#include <pthread.h>
#include <malloc.h>

#define SIZE 1024

void error(char* msg){
	perror(msg);
	exit(0);
}

int main(){
	int sfd;
	struct sockaddr_in servaddr;
	int port_no;

	printf("Enter port number at which you want to connect.\n1.Addition : 8080\n2.Subtraction : 8081\n3.Multiplication : 8082\n4.Modulus : 8083\n");
	scanf("%d",&port_no);

	sfd = socket(AF_INET,SOCK_STREAM,0);
	if(sfd < 0){
		error("Socket sfd error : ");
	}

	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port_no);
	inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);

	connect(sfd,(struct sockaddr*)&servaddr,sizeof(servaddr));

	char a[10],b[10];
	printf("Enter two numbers : ");
	scanf("%s %s",a,b);

	write(sfd,a,10);
	write(sfd,b,10);

	int res;
	read(sfd,&res,10);

	printf("result is : %d\n",res);

	close(sfd);

}