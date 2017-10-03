#ifndef _H_RUNTIME
#define _H_RUNTIME

#include "metadata/Class.h"
#include "metadata/Thread.h"
#include "metadata/Monitor.h"
#include "structures/Heap.h"

typedef struct runtime
{
	Class** classes;
	int numClasses;
	struct thread** threads;
	int numThreads;
	Monitor* threadsMon;
	struct heap* heap;
	Monitor* heapMon;
} Runtime;

typedef struct
{
	Runtime* runtime;
	int currPos;
} ClassIter;

Runtime* newRuntime(Class* mainClass);
void freeAllRuntime(Runtime* runtime);

void parseClasspath(Runtime* runtime, char* classpath);
Class* getClass(Runtime* runtime, char* name);
struct thread* getThread(Runtime* runtime, int tid);
int getNumClass(Runtime* runtime);
int getNumThread(Runtime* runtime);
void addClass(Runtime* runtime, Class* class);
void addThread(Runtime* runtime, struct thread* thread);

ClassIter* newClassIter(Runtime* runtime);
void freeClassIter(ClassIter* iter);
Class* nextClass(ClassIter* iter);
int hasNextClass(ClassIter* iter);

#endif
