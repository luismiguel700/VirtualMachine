#include "Heap.h"
#include "../metadata/Type.h"
#include <stdio.h>

#define SIZE 10000

void mark(Heap* heap, Thread** threads, int numThreads);
void markFrame(Heap* heap, Frame* frame);
void markFrameVars(Heap* heap, Frame* frame);
void markFrameArgs(Heap* heap, Frame* frame);
void markFrameStack(Heap* heap, Stack* stack);
void markIndex(Heap* heap, int index);
int indexOf(Heap* heap, void* value);
void sweep(Heap* heap);
void** sweepArray(int* areMarked, void** array, int* num, int size, void (*freeVal)(void*));
void freeHeapArray(void** array, int num, void (*freeVal)(void*));
int isHeapVal(Type* t);

//nao esquecer: mudar para HASHTABLE

Heap* newHeap()
{
	Heap* res = (Heap*)malloc(sizeof(Heap));
	res->objs = (Object**)malloc(sizeof(Object*) * SIZE);
	res->mons = (Monitor**)malloc(sizeof(Monitor*) * SIZE);
	res->arrays = (Array**)malloc(sizeof(Array*) * SIZE);
	res->numObjs = 0;
	res->numMons = 0;
	res->numArrays = 0;
	res->areMarked = (int*)malloc(sizeof(int) * 3 * SIZE);
	
	return res;
}

void freeAllHeap(Heap* heap)
{
	freeHeapArray(heap->objs, heap->numObjs, freeObject);
	free(heap->objs);
	freeHeapArray(heap->mons, heap->numMons, freeAllMonitor);
	free(heap->mons);
	freeHeapArray(heap->arrays, heap->numArrays, freeArray);
	free(heap->arrays);
	free(heap->areMarked);
	free(heap);
}

void freeHeapArray(void** array, int num, void (*freeVal)(void*))
{
	int i;

	for(i=0; i<num; i++)
		freeVal(array[i]);
}

Object* addObj(Heap* heap, Class* class)
{
	Object* obj = newObject(class);
	heap->objs[heap->numObjs] = obj;
	heap->areMarked[heap->numObjs] = 0;
	heap->numObjs++;

	return obj;
}

Monitor* addMon(Heap* heap)
{
	Monitor* mon = newMonitor();
	heap->mons[heap->numMons] = mon;
	heap->areMarked[SIZE + heap->numMons] = 0;
	heap->numMons++;

	return mon;
}

Array* addArray(Heap* heap, int size, Type* elemType)
{
	Array* array = newArray(size, elemType);
	heap->arrays[heap->numArrays] = array;
	heap->areMarked[SIZE*2 + heap->numArrays] = 0;
	heap->numArrays++;

	return array;
}

void doGC(Heap* heap, struct thread** threads, int numThreads)
{
	mark(heap, threads, numThreads);
	sweep(heap);
}

void mark(Heap* heap, Thread** threads, int numThreads)
{
	int i;

	for(i=0; i<numThreads; i++)
	{
		StackIter* iter = getFramesIter(threads[i]);

		while(hasNext(iter))
		{
			Frame* currFrame = (Frame*)next(iter);
			markFrameArgs(heap, currFrame);
			markFrameVars(heap, currFrame);
			markFrameStack(heap, getStack(currFrame));
		}
	}
}

void markFrameArgs(Heap* heap, Frame* frame)
{
	int numArgs = getNumArgs(frame);
	int i;

	for(i=0; i<numArgs; i++)
		markIndex(heap, indexOf(heap, getArg(frame, i)));
}

void markFrameVars(Heap* heap, Frame* frame)
{
	int numVars = getNumVars(frame);
	int i;
	
	for(i=0; i<numVars; i++)
		markIndex(heap, indexOf(heap, getVar(frame, i)));
}

void markFrameStack(Heap* heap, Stack* stack)
{
	StackIter* iter = newStackIter(stack);

	while(hasNext(iter))
		markIndex(heap, indexOf(heap, (Object*)next(iter)));
}

//alterar
void markIndex(Heap* heap, int index)
{
	if(index != -1)
	{
		heap->areMarked[index] = 1;

		if(index >= 0 && index < SIZE)
		{
			Object* obj = heap->objs[index];
			FieldsIter* iter = newFieldsIter(obj);
			FieldsTypesIter* iterTypes = newFieldsTypesIter(getClassObj(obj));

			while(hasNextField(iter))
				if(isHeapVal(nextFieldType(iterTypes)))
					markIndex(heap, indexOf(heap, nextField(iter)));
		}
		else if(index >= SIZE*2 && index < SIZE*3)
		{
			Array* array = heap->arrays[index - SIZE*2];
			int i;

			if(isHeapVal(getElementType(array)))
				for(i=0; i<size(array); i++)
					markIndex(heap, indexOf(heap, getElem(array, i)));
		}
	}
}

int indexOf(Heap* heap, void* value)
{
	int i;
	
	for(i=0; i<heap->numObjs; i++)
		if(heap->objs[i] == value)
			return i;

	for(i=0; i<heap->numMons; i++)
		if(heap->mons[i] == value)
			return SIZE + i;

	for(i=0; i<heap->numArrays; i++)
		if(heap->arrays[i] == value)
			return SIZE*2 + i;
			
	return -1;
}

void sweep(Heap* heap)
{
	heap->objs = (Object**)sweepArray(heap->areMarked, (void**)heap->objs, &(heap->numObjs), 0, freeObject);
	heap->mons = (Monitor**)sweepArray(heap->areMarked, (void**)heap->mons, &(heap->numMons), SIZE, freeAllMonitor);
	heap->arrays = (Array**)sweepArray(heap->areMarked, (void**)heap->arrays, &(heap->numArrays), 2*SIZE, freeArray);
}

void** sweepArray(int* areMarked, void** array, int* num, int size, void (*freeVal)(void*))
{
	void** newArray = (void**)malloc(sizeof(void*) * SIZE);
	int numNew = 0;
	int i;

	for(i=0; i<(*num); i++)
		if(!areMarked[size+i])
			freeVal(array[i]);
		else
		{
			areMarked[i] = 0;
			newArray[numNew] = array[i];
			numNew++;
		}
	
	free(array);
	*num = numNew;
	return newArray;
}

int isHeapVal(Type* type)
{
	return type->typeCode == T_CLASS || type->typeCode == T_LOCK || type->typeCode == T_ARRAY;
}
