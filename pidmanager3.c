#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "pidmanager.h"

struct pids * pidmanager = 0;
pthread_mutex_t alloc_mutex;

int allocate_map(void){
    int i;
    
    if(pidmanager!=0)
	return 1;
	
    pthread_mutex_init(&alloc_mutex,0);
    pidmanager = (struct pids *)malloc(sizeof(struct pids));
    
    if(pidmanager){
	pidmanager->last_pid = MIN_PID;
	for(i = 0; i < MAX_PAGE; i ++){
	    pidmanager->map[i].num_free = BITS_PER_PAGE;
	    pidmanager->map[i].page = 0;
	}
	pidmanager->map[MIN_PID/BITS_PER_PAGE].num_free = BITS_PER_PAGE - MIN_PID%BITS_PER_PAGE;
    }else{
	return -1;
    }

    return 1;
}

int get_next_free(void){
    int page,offset, first,mask;
    
    if(pidmanager == 0)
	return -1;

    first = pidmanager->last_pid / BITS_PER_PAGE;
    for(page = first; page < MAX_PAGE; page++){
	if(pidmanager->map[page].num_free > 0)
	    break;
    }

    if(page == MAX_PAGE)
	return -1;
    
    if(first * BITS_PER_PAGE < MIN_PID)
	first = MIN_PID % BITS_PER_PAGE;
    else
	first = 0;
    
    for(offset = first; offset < BITS_PER_PAGE; offset++){
	mask = 1 << offset;
	if((pidmanager->map[page].page & mask) == 0)
	    break;
    }
    
    if(offset == BITS_PER_PAGE){
	fprintf(stdout,"Something wrong! last pid %d page %u \n",pidmanager->last_pid,pidmanager->map[page].page);
    }
    
    return page * BITS_PER_PAGE + offset;
}

int allocate_pid(void){
    int pid = -1;
    int offset,page,mask;
    
    if(pidmanager == 0)
	return -1;
    
    pthread_mutex_lock(&alloc_mutex);
    pid = pidmanager->last_pid;
    pidmanager->last_pid++;
    if(pidmanager->last_pid>MAX_PID){
	pthread_mutex_unlock(&alloc_mutex);
	return -1; //no more free
    }
    
    page = pid / BITS_PER_PAGE;
    offset = pid % BITS_PER_PAGE;
    mask = 1 << offset;
    
    if((pidmanager->map[page].page & mask) == 0){
	pidmanager->map[page].page = pidmanager->map[page].page | mask;
	pidmanager->map[page].num_free--;
    }else{
	pid = get_next_free();
	page = pid / BITS_PER_PAGE;
	offset = pid % BITS_PER_PAGE;
	mask = 1 << offset;
	pidmanager->map[page].page = pidmanager->map[page].page | mask;
	pidmanager->map[page].num_free--;
	pidmanager->last_pid = pid + 1;
    }
    pthread_mutex_unlock(&alloc_mutex);
    return pid;
}

void release_pid(int pid){
    int page,offset,mask;
    
    page = pid / BITS_PER_PAGE;
    offset = pid % BITS_PER_PAGE;
    mask = ~(1 << offset);
    pthread_mutex_lock(&alloc_mutex);
    pidmanager->map[page].page = pidmanager->map[page].page & mask;
    pidmanager->map[page].num_free++;
    if(pidmanager->last_pid > pid)
	pidmanager->last_pid = pid;
    pthread_mutex_unlock(&alloc_mutex);
}




