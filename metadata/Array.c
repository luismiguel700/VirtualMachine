#include "Array.h"
#include <stdlib.h>

Array* newArray(int size, Type* elemType)
{
	Array* res = (Array*)malloc(sizeof(Array));
	res->elems = (void**)malloc(sizeof(void*) * size);
	res->size = size;
	res->elemType = elemType;
	
	return res;
}

void freeArray(Array* array)
{
	free(array->elems);
	free(array);
}

void* getElem(Array* array, int i)
{
	return array->elems[i];
}

void setElem(Array* array, int i, void* val)
{
	array->elems[i] = val;
}

int size(Array* array)
{
	return array->size;
}

Type* getElementType(Array* array)
{
	return array->elemType;
}
