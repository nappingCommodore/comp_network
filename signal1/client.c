#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>
#include  <sys/ipc.h>
#include  <sys/shm.h>
#include  <unistd.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include  <malloc.h>

void  main(void)
{
     pid_t   pid;
     key_t MyKey,key;
     int   ShmID,shmID1;
     pid_t *ShmPTR;
     char  line[100], c;
     int   i;

     MyKey   = ftok(".", 's');        
     ShmID   = shmget(MyKey, sizeof(pid_t), 0666);
     ShmPTR  = (pid_t *) shmat(ShmID, NULL, 0);
     pid     = *ShmPTR;                
     shmdt(ShmPTR);                    
	
	

     while (1) {                       
          printf("Want to interrupt the other guy or kill it or enter a msg(i or k or m)? ");
          gets(line);
          for (i = 0; !(isalpha(line[i])); i++)
               ;
               c = line[i];
          if (c == 'i' || c == 'I') {  
               kill(pid, SIGINT);
               printf("Sent a SIGINT signal\n");
          }
          else if (c == 'k' || c == 'K') {
               printf("About to send a SIGQUIT signal\n");
               kill(pid, SIGQUIT);     
               printf("Done.....\n");
               exit(0);
          }
          else if(c == 'm' || c == 'M'){
          	char* msg = (char*) malloc(sizeof(char)*1024);
          	char* myfifo = "/home/niks/my_codes/comp_network/signal1/myfifo";
          	int fd = open(myfifo,O_WRONLY);
          	printf("Enter yout msg.\n");
          	scanf("%s",msg);
          	write(fd,msg,1024);
          	close(fd);
          }
          else
               printf("Wrong keypress (%c).  Try again\n", c);
     }
}
