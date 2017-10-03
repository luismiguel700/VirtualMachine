#ifndef _H_STACK
#define _H_STACK

#define SIZE 100

typedef struct stack
{ 
	void** elems; 
	int number;
} Stack;

typedef struct
{
	Stack* stack;
	int currPos;
} StackIter;

Stack* newStack(int size);
void freeStack(Stack* stack);
int equalStack(Stack* s1, Stack* s2, int (*equal)(void*, void*));
Stack* cloneStack(Stack* s);

void* peek(Stack* stack);
void* pop(Stack* stack);
void push(Stack* stack, void* elem);
int getNumber(Stack* stack);

StackIter* newStackIter(Stack* stack);
void freeAllStackIter(StackIter* iter);

int hasNext(StackIter* iter);
void* next(StackIter* iter);

#endif
