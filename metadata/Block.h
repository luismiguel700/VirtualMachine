#include "../util/Stack.h"
#include "Method.h"

#ifndef _H_BLOCK
#define _H_BLOCK

typedef struct
{
	char* label;
	Stack* stackType;
	char* code;
	int numBytes;
} Block;

Block* newBlock(char* label, Stack* stackType);
void freeAllBlock(Block* block);

char* getLabel(Block* block);
Stack* getStackType(Block* block);
void addByte(Block* block, char byte);
char getByte(Block* block, int i);
int getNumBytes(Block* block);

#endif
