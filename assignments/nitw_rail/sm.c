//train ports 8080,8081,8082 platform ports 9090,9091,9092


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>

#define SIZE 1024

int flag[3] = {0};

void e_error(char* msg){
	perror(msg);
	exit(0);
}

void platforms(int ports[]){
	printf("Inside platform\n");
	int pid;
	pid = fork();
	char ch[10];
	sprintf(ch,"%d",ports[0]);
	if(pid == 0){
		printf("plateform1 is opening\n");
		sprintf(ch,"%d",ports[0]);
		if(execl("pl","pl",ch,NULL) == -1)
			e_error("execl() error");
	}
	else{

	}

	pid = fork();
	if(pid == 0){
		printf("plateform2 is opening\n");
		sprintf(ch,"%d",ports[1]);
		if(execl("pl","pl",ch,NULL) == -1)
			e_error("execl() error");
	}
	else{

	}

	pid = fork();
	if(pid == 0){
		printf("plateform3 is opening\n");
		sprintf(ch,"%d",ports[2]);
		if(execl("pl","pl",ch,NULL) == -1)
			e_error("execl() error");
	}
	else{

	}
	printf("Exiting plateform\n");
}


int check_avail(){
	int i;
	for(i=0;i<3;i++){
		if(flag[i] == 0)
			return i;
	}
	return -1;
}


void sig_handler1(int arg){
	printf("Signal1 caught\n");
	flag[0] = 0;
}

void sig_handler2(int arg){
	printf("Signal2 caught\n");
	flag[1] = 0;
}

void sig_handler3(int arg){
	printf("Signal3 caught\n");
	flag[2] = 0;
}

void set(fd_set *readfds,int arr[],int p){
	int i;
	FD_ZERO(readfds);
	for(i=0;i<p;i++){
		FD_SET(arr[i],readfds);
	}
} 

int main(int argc , char* argv[]){

	int sfd[3],nsfd,max_fd,i;
	fd_set readfds,writefds;
	struct sockaddr_in servaddr[3];
	int port[] = {atoi(argv[1]),atoi(argv[1])+1,atoi(argv[1])+2};	//train-sm
	int ports[] = {atoi(argv[2]),atoi(argv[2])+1,atoi(argv[2])+2};	//trin-pl
	bzero((struct sockaddr*)&servaddr,sizeof(servaddr));

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	signal(SIGUSR1,sig_handler1);
	signal(SIGUSR2,sig_handler2);
	signal(SIGQUIT,sig_handler3);
	
	max_fd = 0;

	int arr[SIZE];
	int p = 0;

	for(i=0;i<3;i++){	//created three servaddr structures and created three sockets and added into readfds
		sfd[i] = socket(AF_INET,SOCK_STREAM,0);
		//printf("sfd [%d] %d\n",i,sfd[i]);
		arr[p++] = sfd[i];
		if(sfd[i] < 0){
			e_error("socket() error");
		}

		bzero((struct sockaddr_in*)&servaddr,sizeof(servaddr));

		printf("sfd[%d] %d \n",i,sfd[i]);
		if(sfd[i] > max_fd){
			max_fd = sfd[i];
		}
		FD_SET(sfd[i],&readfds);
		servaddr[i].sin_family = AF_INET;
		servaddr[i].sin_addr.s_addr = htons(INADDR_ANY);
		servaddr[i].sin_port = htons(port[i]);

		//printf()

		int x = bind(sfd[i],(struct sockaddr*)&servaddr,sizeof(servaddr));
		if(x < 0){
			printf("%d\n",port[i]);
			e_error("bind() error in sm");
		}

		x = listen(sfd[i],10);
		if(x < 0){
			e_error("listen() error");
		}
		//close(sfd[i]);
	}

	struct timeval to;
	to.tv_sec = 10;
	to.tv_usec = 0;

	printf("%d\n",max_fd);

	platforms(ports);
	while(1){
		int i;
		set(&readfds,arr,p);
		int ret = select(max_fd+1,&readfds,NULL,NULL,&to);
		printf("%d\n",ret);
		//sleep(2);
		if(ret > 0){
			for(i=0;i<=max_fd;i++){
				printf("%d\n",i);
				if(FD_ISSET(i,&readfds)){
					//if(i == sfd){
						int len = sizeof(servaddr);
						nsfd = accept(i,(struct sockaddr*)&servaddr,&len);
						if(nsfd < 0)
							e_error("accept() error");
						printf("port %d is open\n",i);

						if(max_fd < nsfd){
							max_fd = nsfd;
						}
						//FD_SET(nsfd,&readfds);
						arr[p++] = nsfd;
						int chk = check_avail();
						printf("%d\n",chk);
						if(chk == -1){
							printf("No platform available...\n");
						}
						else{
							flag[chk] = 1;
							write(sfd[i],&ports[chk],SIZE);
						}
					//}
				}
			}
		}
	}
}