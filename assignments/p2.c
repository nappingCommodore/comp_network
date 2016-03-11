#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	while(1){
		srand(time(NULL));
		int x = ((double)rand()/RAND_MAX)*5;
		printf("Hello from p2..\n");
		sleep(x);
	}
}