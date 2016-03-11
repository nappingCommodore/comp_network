#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define SIZE 1024

int main(){
	int pid;
	int fd[2];

	pipe(fd);	//ppipe created

	char msg[SIZE] = "hello";

	pid = fork();
		if(pid == 0){	//child process
			printf("child \n");
			close(fd[1]);
			dup2(fd[0],0);	//0 stdin is attached to fd[0] (read fd)
			//write(fd[1],a,1024);
			execl("cl","-c",NULL);
		}

		else{
			wait();
			close(fd[0]);
			//printf("Enter msg in parent\n");
			//scanf("%s",msg);

			write(fd[1],msg,SIZE);
			//printf("%s\n",msg);
		}
}