#include <stdio.h>

int main(){
	int fd[2];	//fd[0] = read end,  fd[1] = write end
	pipe(fd);

	int pid = 0;	
	pid = fork();

	if(pid == 0){	//write
		close(fd[0]);	//read
		char msg[10] = "hello";
		write(fd[1],msg,10);
	}

	else{	//read
		wait();
		close(fd[1]);
		char msg[10];
		read(fd[0],msg,10);
		printf("%s\n",msg);
	}

	return 0;
}