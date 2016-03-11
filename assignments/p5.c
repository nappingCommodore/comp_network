#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(){
	while(1){
		srand(time(NULL));
		int x = ((double)rand()/RAND_MAX)*5;
		sleep(x);
		printf("Hello from p5.....\n");
	}
}