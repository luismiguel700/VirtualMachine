#ifndef _H_MONITOR
#define _H_MONITOR

#include <pthread.h>

typedef struct
{
	pthread_mutex_t* lock;
	pthread_cond_t* canRead;
	pthread_cond_t* canWrite;
	int count;
} Monitor;

Monitor* newMonitor();
void freeAllMonitor(Monitor* mon);

void startShared(Monitor* mon);
void endShared(Monitor* mon);
void startSync(Monitor* mon);
void endSync(Monitor* mon);

#endif
