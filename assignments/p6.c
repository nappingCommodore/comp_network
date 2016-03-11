#include <stdio.h>

int main(){
	while(1){
		char msg[100];
		fgets(msg,100,stdin);
		fputs(msg,stdout);
		//sleep(1);
	}
}