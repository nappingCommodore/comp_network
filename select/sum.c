#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <fcntl.h>

int main(){
	int fd = open("fifo",O_RDONLY);
	int a,b;
	read(fd,a,1024);
	read(fd,b,1024);

	printf("%d\n",a+b);
	return 0;
}