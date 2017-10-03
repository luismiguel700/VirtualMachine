#include "Block.h"
#include <stdlib.h>

#define SIZE 100

Block* newBlock(char* label, Stack* stackType)
{
	Block* res = (Block*)malloc(sizeof(Block));
	res->label = label;
	res->stackType = stackType;
	res->code = (char*)malloc(sizeof(char)*SIZE);
	res->numBytes = 0;

	return res;
}

void freeAllBlock(Block* block)
{
	free(block->code);
	free(block);
}

char* getLabel(Block* block)
{
	return block->label;
}

Stack* getStackType(Block* block)
{
	return block->stackType;
}

void addByte(Block* block, char byte)
{
	block->code[block->numBytes] = byte;
	block->numBytes++;
}

char getByte(Block* block, int i)
{
	return block->code[i];
}

int getNumBytes(Block* block)
{
	return block->numBytes;
}
