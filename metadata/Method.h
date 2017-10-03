#ifndef _H_METHOD
#define _H_METHOD

#include "Signature.h"
#include "Type.h"
#include "Block.h"

typedef struct
{
	Signature* sig;
	Type** varTypes;
	int numVarTypes;
	Block** blocks;
	int numBlocks;
} Method;

typedef struct
{
	Method* method;
	int currPos;
} BlockIter;

Method* newMethod(Signature* sig);
void freeAllMethod(Method* method);

Signature* getSignature(Method* method);
void addVarType(Method* method, Type* type);
Type* getVarType(Method* method, int i);
int getNumVarTypes(Method* method);
void addBlock(Method* method, Block* block);
Block* getBlock(Method* method, char* label);
Block* getFirstBlock(Method* method);

BlockIter* newBlockIter(Method* method);
void freeBlockIter(BlockIter* iter);

int hasNextBlock(BlockIter* iter);
Block* nextBlock(BlockIter* iter);

#endif
