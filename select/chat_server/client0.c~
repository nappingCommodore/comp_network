#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/times.h>
#include <fcntl.h>
#include <string.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <stdint.h>

#define SIZE 1024

struct shared_mem{
	bool flag;
	int sender;
	int receiver;
	int clients;
};

int shmid;
struct shared_mem* shm;
pthread_t *reader_threads,*writer_threads;
pthread_cond_t *write_cond,*read_cond;
pthread_mutex_t *write_mutex,*read_mutex;

void Error(char *msg){
	perror("Error ");
	printf("%s \n",msg);
	exit(0);
}


void init(){
	if((shmid = shmget(1234,SIZE,0666|IPC_CREAT)) < 0){
		Error("shmget");
	}
	
	shm = (struct shared_mem*)shmat(shmid,NULL,0);
	
	if(shm == (struct shared_mem*)-1){
		Error("shmat");
	}
	
	reader_threads = (pthread_t*) malloc(sizeof(pthread_t)*shm->clients);
	writer_threads = (pthread_t*) malloc(sizeof(pthread_t)*shm->clients);
	
	read_cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t)*shm->clients);
	write_cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t)*shm->clients);
	
	read_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*shm->clients);
	write_mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*shm->clients);
	
	int i;
	for(i=0;i<shm->clients;i++){
		pthread_cond_init(&write_cond[i],NULL);
		pthread_cond_init(&read_cond[i],NULL);
		pthread_mutex_init(&write_mutex[i],NULL);
		pthread_mutex_init(&read_mutex[i],NULL);
	}
}


void* reader(void* ptr){
	int *i = (int*) malloc(sizeof(int));
	i = (int*)ptr;
	
	pthread_mutex_lock(&read_mutex[*i]);
	if(shm->receiver != *i){
		pthread_cond_wait(&read_cond[*i],&read_mutex[*i]);
	}
	
	else{
		pthread_cond_signal(&read_cond[*i]);
	}
	pthread_mutex_unlock(&read_mutex[*i]);
	
	printf("Critical section reader has been ended..\n");
	
	char myfifo[SIZE],msg[SIZE];
	snprintf(myfifo,SIZE,"myfifo%d",shm->sender);
	int fd = open(myfifo,O_RDONLY);
	
	read(fd,msg,SIZE);
	
	printf("Client %d read msg \" %s \" from Server %d\n",shm->receiver,msg,shm->sender);
	
	close(fd);
}

void* writer(void* ptr){
	int *i = (int*) malloc(sizeof(int));
	i = (int*)ptr;
	
	
	pthread_mutex_lock(&write_mutex[*i]);
	if(shm->sender != *i){
		pthread_cond_wait(&write_cond[*i],&write_mutex[*i]);
	}
	
	else{
		shm->flag = false;
		pthread_cond_signal(&write_cond[*i]);
	}
	pthread_mutex_unlock(&write_mutex[*i]);
	
	printf("critical section writer has been ended..\n");
	
	char myfifo[SIZE],msg[SIZE];
	snprintf(myfifo,SIZE,"myfifo%d",shm->sender);
	int fd = open(myfifo,O_WRONLY);
	
	printf("Enter your msg.\n");
	scanf("%s",msg);
	
	write(fd,msg,SIZE);
	close(fd);
}

int main(){
	init();
	printf("%d\n",shm->clients);
	int i;
	while(1){
		for(i=0;i<shm->clients;i++){
			pthread_create(&writer_threads[i],NULL,writer,&i);
			pthread_create(&reader_threads[i],NULL,reader,&i);
		}
	
		for(i=0;i<shm->clients;i++){
			pthread_join(reader_threads[i]);
			pthread_join(writer_threads[i]);
		}
	}
}



