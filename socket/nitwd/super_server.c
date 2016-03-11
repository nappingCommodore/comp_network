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
#include <fcntl.h>

#define SIZE 1024



struct thread_struct{
	int sfd;
	struct sockaddr_in servaddr;
	int service_no;
};



void error(const char* msg){
	perror(msg);
	exit(0);
}



void* thread_handler(void* arg){
	struct thread_struct ts = *(struct thread_struct*)arg;
	int len = sizeof(ts.servaddr);
	char **argv = (char**)malloc(sizeof(char*)*10);
	int i;
	for(i=0;i<10;i++){
		argv[i] = (char*)malloc(sizeof(char)*10);
	}
	int nsfd = accept(ts.sfd,(struct sockaddr*) &ts.servaddr,&len);
	printf("Client connected at port %d...\n",ts.servaddr.sin_port);
	int fifo_fd;

	printf("%d\n",nsfd);

	if(nsfd > 0){
		read(nsfd,argv[1],10);
		read(nsfd,argv[2],10);
		argv[3] = NULL;
		printf("Accept successful.\n");
		int pid = fork();
		if(pid == 0){
			close(ts.sfd);
			//dup2(nsfd,STDIN_FILENO);
			char* service = (char*) malloc(sizeof(char)*SIZE);
			switch(ts.service_no){
				case 1: printf("client have been granted service number 1 (Addition).\n");
						service = "sum";
						mkfifo("sumfifo",0666);
						fifo_fd = open("sumfifo",O_RDWR);
						break;

				case 2: printf("client have been granted service number 2 (Subtraction).\n");
						service = "sub";
						mkfifo("subfifo",0666);
						fifo_fd = open("subfifo",O_RDWR);
						break;

				case 3: printf("client have been granted service number 3 (Multiplication).\n");
						service = "mul";
						mkfifo("mulfifo",0666);
						fifo_fd = open("mulfifo",O_RDWR);
						break;

				case 4: printf("client have been granted service number 4 (Modulus).\n");
						service = "mod";
						mkfifo("modfifo",0666);
						fifo_fd = open("modfifo",O_RDWR);
						break;

				default: break;
			}
			execv(service,argv);
		}

		else{
			//close(nsfd);
			sleep(5);
			printf("sleep ends now..\n");
			int res;
			read(fifo_fd,&res,10);
			printf("res (super_server) %d\n",res);
			write(nsfd,&res,10);
			write(ts.sfd,&res,10);
			close(fifo_fd);
			close(nsfd);
			close(ts.sfd);
			//unlink("sumfifo");
		}
	}
}



int main(){
	int sfd1,sfd2,sfd3,sfd4,nsfd;
	struct sockaddr_in servaddr1,servaddr2,servaddr3,servaddr4;
	int i,j;
	fd_set readfds;
	struct timeval timeout;
	pthread_t thread1,thread2,thread3,thread4;
	struct thread_struct ts1,ts2,ts3,ts4;

	sfd1 = socket(AF_INET,SOCK_STREAM,0);
	sfd2 = socket(AF_INET,SOCK_STREAM,0);
	sfd3 = socket(AF_INET,SOCK_STREAM,0);
	sfd4 = socket(AF_INET,SOCK_STREAM,0);

	if(sfd1 < 0){
		error("socket sfd1 error : ");
	}

	if(sfd2 < 0){
		error("socket sfd2 error : ");
	}

	if(sfd3 < 0){
		error("socket sfd3 error : ");
	}

	if(sfd4 < 0){
		error("socket sfd4 error : ");
	}

	bzero(&servaddr1,sizeof(servaddr1));
	bzero(&servaddr2,sizeof(servaddr2));
	bzero(&servaddr3,sizeof(servaddr3));
	bzero(&servaddr4,sizeof(servaddr4));


	servaddr1.sin_family = AF_INET;
	servaddr1.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr1.sin_port = htons(8080);

	servaddr2.sin_family = AF_INET;
	servaddr2.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr2.sin_port = htons(8081);

	servaddr3.sin_family = AF_INET;
	servaddr3.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr3.sin_port = htons(8082);

	servaddr4.sin_family = AF_INET;
	servaddr4.sin_addr.s_addr = htons(INADDR_ANY);
	servaddr4.sin_port = htons(8083);

	int x = bind(sfd1,(struct sockaddr*)&servaddr1,sizeof(servaddr1));
	if(x < 0){
		error("bind error in serv1 : ");
	}

	x = bind(sfd2,(struct sockaddr*) &servaddr2,sizeof(servaddr2));
	if(x < 0){
		error("bind error in serv2 : ");
	}

	x = bind(sfd3,(struct sockaddr*) &servaddr3,sizeof(servaddr3));
	if(x < 0){
		error("bind error in serv3 : ");
	}

	x = bind(sfd4,(struct sockaddr*) &servaddr4,sizeof(servaddr4));
	if(x < 0){
		error("bind error in serv4 : ");
	}

	x = listen(sfd1,5);	//max 5 clients
	if(x < 0){
		error("listen error in serv1 : ");
	}

	x = listen(sfd2,4);
	if(x < 0){
		error("listen error in serv2 : ");
	}

	x = listen(sfd3,3);
	if(x < 0){
		error("listen error in serv3 : ");
	}

	x = listen(sfd4,2);
	if(x < 0){
		error("listen error in serv4 : ");
	}

	FD_ZERO(&readfds);

	FD_SET(sfd1,&readfds);
	FD_SET(sfd2,&readfds);
	FD_SET(sfd3,&readfds);
	FD_SET(sfd4,&readfds);

	timeout.tv_sec = 3;
	timeout.tv_usec = 0;

	int rel = select(FD_SETSIZE,&readfds,NULL,NULL,NULL);
	if(rel > 0){
		ts1.sfd = sfd1;
		ts1.servaddr = servaddr1;
		ts1.service_no = 1;

		ts2.sfd = sfd2;
		ts2.servaddr = servaddr2;
		ts2.service_no = 2;

		ts3.sfd = sfd3;
		ts3.servaddr = servaddr3;
		ts3.service_no = 3;

		ts4.sfd = sfd4;
		ts4.servaddr = servaddr4;
		ts4.service_no = 4;

		pthread_create(&thread1,NULL,thread_handler,&ts1);
		pthread_create(&thread2,NULL,thread_handler,&ts2);
		pthread_create(&thread3,NULL,thread_handler,&ts3);
		pthread_create(&thread4,NULL,thread_handler,&ts4);

		pthread_join(thread1,NULL);
		pthread_join(thread2,NULL);
		pthread_join(thread3,NULL);
		pthread_join(thread4,NULL);


		printf("Exiting.\n");

	}

}