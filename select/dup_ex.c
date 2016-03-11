#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
	int pid;
	pid = fork();

	mkfifo("fifo",0777);
	int fd = open("fifo",O_WRONLY);

	if(pid == 0){
		int a,b;
		scanf("%d %d",&a,&b);
		write(fd,a,1024);
		write(fd,b,1024);
		execl("sum","-c",NULL);
	}
	wait();
}