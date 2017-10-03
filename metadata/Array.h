#ifndef _H_ARRAY
#define _H_ARRAY

#include "Type.h"

typedef struct
{
	void** elems;
	int size;
	Type* elemType;
} Array;

Array* newArray(int size, Type* elemType);
void freeArray(Array* array);

void* getElem(Array* array, int i);
void setElem(Array* array, int i, void* val);
int size(Array* array);
Type* getElementType(Array* array);

#endif
