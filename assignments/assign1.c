#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 1024

int main(){
	FILE* p[5];//,*p3,*p4,*p5,*p6;
	struct pollfd pfds[SIZE];
	int i,timeout_msecs = 5000;

	char ch[30];
	for(i=0;i<4;i++){
		snprintf(ch,30,"./p%d",i);
		p[i] = popen(ch,"r");
	}

	/*p2 = popen("./p2","r");
	p3 = popen("./p3","r");
	p4 = popen("./p4","r");
	p5 = popen("./p5","r");*/

	p[4] = popen("./p4","w");

	for(i=0;i<4;i++){
		pfds[i].fd = fileno(p[i]);
		pfds[i].events = POLLIN;
	}

	/*pfds[1].fd = fileno(p3);
	pfds[1].events = POLLIN;

	pfds[2].fd = fileno(p4);
	pfds[2].events = POLLIN;

	pfds[3].fd = fileno(p5);
	pfds[3].events = POLLIN;*/

	pfds[4].fd = fileno(p[4]);
	pfds[4].events = POLLOUT;

	char msg[SIZE];

	while(1){
		int ret = poll(pfds,5,timeout_msecs);

		if(ret > 0){
			int flag = 0;
			for(i=0;i<4;i++){
				if(pfds[i].revents & POLLIN){
					//char f[10];
					//snprintf(f,10,"p%d",i+2);
					//if(pfds[4].revents & POLLOUT){
						fgets(msg,SIZE,p[i]);
						fputs(msg,p[4]);
						flag = 1;
					//}
				}
			}
			if(flag == 0){
				srand(time(NULL));
				int x = ((double)rand()/RAND_MAX)*5;
				fputs("Hello from p1.\n",p[4]);
				sleep(x);
			}
		}
	}

	return 0;
}