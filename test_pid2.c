#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "pidmanager.h"

#define NUM_THREAD 100
#define MAX_WAIT 5

void * exec_thread(void * d){
    int child_num = * (int *) d;
    int pid = allocate_pid();
    fprintf(stdout,"Child #%d has pid %d\n",child_num,pid);
    sleep(rand()%MAX_WAIT);
    fprintf(stdout,"Child #%d release pid %d\n",child_num,pid);
    release_pid(pid);
    return 0;
}

int main(int argc, char * argv[]){
    pthread_t thread[NUM_THREAD];
    int thread_num[NUM_THREAD];
    int index;
    srand(time(0));
    if(allocate_map()){
	for(index = 0; index < NUM_THREAD; index++){
	    thread_num[index] = index+1;
	    pthread_create(&thread[index],0,exec_thread,&thread_num[index]);
	}
	for(index = 0; index < NUM_THREAD; index++){
	    pthread_join(thread[index],0);
	}
    }
    
    
    return 0;
}
