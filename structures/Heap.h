#ifndef _H_HEAP
#define _H_HEAP

#include "../metadata/Thread.h"
#include "../metadata/Object.h"
#include "../metadata/Monitor.h"
#include "../metadata/Array.h"
#include "../metadata/Type.h"

typedef struct
{
	Object** objs;
	Monitor** mons;
	Array** arrays;
	int numObjs;
	int numMons;
	int numArrays;
	int* areMarked;
} Heap;

Heap* newHeap();
void freeAllHeap(Heap* heap);

Object* addObj(Heap* heap, Class* class);
Monitor* addMon(Heap* heap);
Array* addArray(Heap* heap, int size, Type* elemType);
void doGC(Heap* heap, struct thread** threads, int numThreads);

#endif
