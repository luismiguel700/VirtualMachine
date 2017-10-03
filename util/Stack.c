#include "Stack.h"
#include <stdlib.h>

Stack* newStack(int size)
{ 
	Stack* res = (Stack*)malloc(sizeof(Stack));
	res->elems = (void**)malloc(sizeof(void*) * size);
	res->number = 0;
	
	return res;
}

void freeStack(Stack* stack)
{
	free(stack->elems);
	free(stack);
}

int equalStack(Stack* s1, Stack* s2, int (*equal)(void*, void*))
{
	if(s1->number != s2->number)
		return 0;

	int i;

	for(i=0; i<s1->number; i++)
		if(!equal(s1->elems[i], s2->elems[i]))
			return 0;

	return 1;
}

Stack* cloneStack(Stack* s)
{
	Stack* res = newStack(SIZE);
	int i;

	for(i=0; i<s->number; i++)
		res->elems[i] = s->elems[i];

	res->number = s->number;

	return res;
}

void* peek(Stack* stack)
{
	return stack->elems[stack->number-1];
}

void* pop(Stack* stack)
{
	stack->number--;
	
	return stack->elems[stack->number];
}

void push(Stack* stack, void* elem)
{
	stack->elems[stack->number] = elem;
	stack->number++;
}

int getNumber(Stack* stack)
{
	return stack->number;
}

StackIter* newStackIter(Stack* stack)
{
	StackIter* res = (StackIter*)malloc(sizeof(StackIter));
	res->stack = stack;
	res->currPos = 0;

	return res;
}

void freeStackIter(StackIter* iter)
{
	free(iter);
}

int hasNext(StackIter* iter)
{
	return iter->currPos < iter->stack->number;
}

void* next(StackIter* iter)
{
	void* res = iter->stack->elems[iter->currPos];
	iter->currPos++;
	
	return res;
}
