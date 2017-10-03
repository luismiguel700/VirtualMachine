#include <stdlib.h>
#include <string.h>
#include "Method.h"

#define SIZE 100

Method* newMethod(Signature* sig)
{
	Method* res = (Method*)malloc(sizeof(Method));
	res->sig = sig;
	res->varTypes = (Type**)malloc(sizeof(Type*) * SIZE);
	res->numVarTypes = 0;
	res->blocks = (Block**)malloc(sizeof(Block*) * SIZE);
	res->numBlocks = 0;
	
	return res;
}

void freeAllMethod(Method* method)
{	
	free(method->sig);
	int i;
	
	for(i=0; i<method->numVarTypes; i++)
		free(method->varTypes[i]);
		
	free(method->varTypes);

	for(i=0; i<method->numBlocks; i++)
		free(method->blocks[i]);

	free(method->blocks);
	free(method);
}

Signature* getSignature(Method* method)
{
	return method->sig;
}

void addVarType(Method* method, Type* type)
{
	method->varTypes[method->numVarTypes] = type;
	method->numVarTypes++;
}

Type* getVarType(Method* method, int i)
{
	return method->varTypes[i];
}

int getNumVarTypes(Method* method)
{
	return method->numVarTypes;
}

void addBlock(Method* method, Block* block)
{
	method->blocks[method->numBlocks] = block;
	method->numBlocks++;
}

Block* getBlock(Method* method, char* label)
{
	int i;

	for(i=0; i<method->numBlocks; i++)
		if(strcmp(getLabel(method->blocks[i]), label) == 0)
			return method->blocks[i];

	return NULL;
}

//pre-condicao: ha pelo menos 1 bloco
Block* getFirstBlock(Method* method)
{
	return method->blocks[0];
}

BlockIter* newBlockIter(Method* method)
{
	BlockIter* res = (BlockIter*)malloc(sizeof(BlockIter));
	res->method = method;
	res->currPos = 0;

	return res;
}

void freeBlockIter(BlockIter* iter)
{
	free(iter);
}

int hasNextBlock(BlockIter* iter)
{
	return iter->currPos < iter->method->numBlocks;
}

Block* nextBlock(BlockIter* iter)
{
	return iter->method->blocks[iter->currPos++];
}
