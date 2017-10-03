#include "Monitor.h"
#include <stdlib.h>
#include <stdio.h>

Monitor* newMonitor()
{
	Monitor* res = (Monitor*)malloc(sizeof(Monitor));
	res->lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	res->canRead = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
	res->canWrite = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
	res->count = 0;
	
	return res;
}

void freeAllMonitor(Monitor* mon)
{
	free(mon->lock);
	free(mon->canRead);
	free(mon->canWrite);
	free(mon);
}

void startShared(Monitor* mon)
{
	pthread_mutex_lock(mon->lock);
	
	while(mon->count == -1)
		pthread_cond_wait(mon->canRead, mon->lock);
	
	mon->count++;
	pthread_cond_signal(mon->canRead);
	pthread_mutex_unlock(mon->lock);
}

void endShared(Monitor* mon)
{
	pthread_mutex_lock(mon->lock);
	mon->count--;
	
	if(mon->count == 0)
		pthread_cond_signal(mon->canWrite);
	
	pthread_mutex_unlock(mon->lock);
}

void startSync(Monitor* mon)
{
	pthread_mutex_lock(mon->lock);
	
	while(mon->count > 0)
		pthread_cond_wait(mon->canWrite, mon->lock);
		
	mon->count = -1;
	pthread_mutex_unlock(mon->lock);
}

void endSync(Monitor* mon)
{
	pthread_mutex_lock(mon->lock);
	mon->count = 0;
	pthread_cond_signal(mon->canRead);
	pthread_cond_signal(mon->canWrite);
	pthread_mutex_unlock(mon->lock);
}
