#include "Thread.h"
#include "../Interpreter.h"
#include <stdio.h>
#include <time.h>

#define SIZE 100

typedef struct
{
	Thread* thread;
	Runtime* runtime;
} ThreadArgs;

void* launchAux(void* arg);

Thread* newThread()
{
	Thread* res = (Thread*)malloc(sizeof(Thread));
	res->frames = newStack(SIZE);
	res->sysThread = (pthread_t*)malloc(sizeof(pthread_t));
	res->alive = 1;

	return res;
}

void launch(Thread* thread, struct runtime* runtime)
{ //passar a funcao a executar
	startSync(runtime->threadsMon);
	addThread(runtime, thread);
	endSync(runtime->threadsMon);

	ThreadArgs* args = (ThreadArgs*)malloc(sizeof(ThreadArgs));
	args->thread = thread;
	args->runtime = runtime;

	pthread_create(thread->sysThread, NULL, launchAux, args);
}

void* launchAux(void* arg)
{
	ThreadArgs* args = (ThreadArgs*)arg;
	void* res = execMethod(args->thread, args->runtime);
	args->thread->alive = 0;
	removeFrame(args->thread);

	return res;
}

void freeThread(Thread* thread)
{
	freeStack(thread->frames);
	free(thread->sysThread);
	free(thread);
}

void* join(Thread* thread)
{
	void** ptr = malloc(sizeof(void*)); //var local
	pthread_join(*(thread->sysThread), ptr);
	void* res = *ptr;
	free(ptr);
	
	return res;
}

int isAlive(Thread* thread)
{
	return thread->alive;
}

void addFrame(Thread* thread, Frame* frame)
{
	push(thread->frames, frame);
}

void removeFrame(Thread* thread)
{
	freeFrame((Frame*)pop(thread->frames));
}

Frame* getFrame(Thread* thread)
{
	return (Frame*)peek(thread->frames);
}

StackIter* getFramesIter(Thread* thread)
{
	return newStackIter(thread->frames);
}

void threadSleep(int seconds)
{
	sleep(seconds);
}
