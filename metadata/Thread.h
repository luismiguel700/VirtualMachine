#ifndef _H_THREAD
#define _H_THREAD

#include "../metadata/Method.h"
#include "../structures/Frame.h"
#include "../Runtime.h"
#include "../util/Stack.h"

#include <pthread.h>

typedef struct thread
{
	Stack* frames;
	pthread_t* sysThread;
	int alive;
} Thread;

Thread* newThread();
void freeThread(Thread* thread);

void launch(Thread* thread, struct runtime* runtime);
void* join(Thread* thread);
int isAlive(Thread* thread);
void addFrame(Thread* thread, Frame* frame);
void removeFrame(Thread* thread);
Frame* getFrame(Thread* thread);
StackIter* getFramesIter(Thread* thread);

void threadSleep(int milliseconds);

#endif
