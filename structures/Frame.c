#include "Frame.h"
#include <stdlib.h>
#include "../metadata/Object.h"

#define SIZE 100

Frame* newFrame(Stack* stack, Signature* sig)
{
	Frame* res = (Frame*)malloc(sizeof(Frame));
	res->numArgs = getNumArgTypes(sig)+1;
	res->args = (void**)malloc(sizeof(void*) * res->numArgs);
	int i;

	for(i=res->numArgs-1; i>=0; i--)
		res->args[i] = pop(stack);

	res->class = getClassObj((Object*)res->args[0]);
	Type* paramType = getParamType((Object*)res->args[0]);

	if(paramType == NULL)
		res->method = getMethod(res->class, sig);
	else
		res->method = getMethodParamType(res->class, sig, paramType);

	res->block = getFirstBlock(res->method);
	res->stack = newStack(SIZE);
	res->vars = (void**)malloc(sizeof(void*) * getNumVarTypes(res->method));
	res->numVars = 0;
	res->ip = 0;
	
	return res;
}

void freeFrame(Frame* frame)
{
	freeStack(frame->stack);
	free(frame->vars);
	free(frame->args);
	free(frame);
}

Class* frameGetClass(Frame* frame)
{
	return frame->class;
}

Method* frameGetMethod(Frame* frame)
{
	return frame->method;
}

Stack* getStack(Frame* frame)
{
	return frame->stack;
}

void setVar(Frame* frame, int i, void* var)
{
	frame->vars[i] = var;
}

void* getVar(Frame* frame, int i)
{
	return frame->vars[i];
}

int getNumVars(Frame* frame)
{
	return frame->numVars;
}

void addArg(Frame* frame, void* arg)
{
	frame->args[frame->numArgs] = arg;
	frame->numArgs++;
}

void* getArg(Frame* frame, int i)
{
	return frame->args[i];	
}

int getNumArgs(Frame* frame)
{
	return frame->numArgs;	
}

Block* getCurrBlock(Frame* frame)
{
	return frame->block;
}

void setCurrBlock(Frame* frame, char* label)
{
	frame->block = getBlock(frame->method, label);
	frame->ip = 0;
}

int getIp(Frame* frame)
{
	return frame->ip;
}

int nextIp(Frame* frame)
{
	return frame->ip++;
}
