#include "Object.h"
#include <stdlib.h>

Object* newObject(Class* class)
{
	Object* res = (Object*)malloc(sizeof(Object));
	res->class = class;
	res->paramType = NULL;
	res->fields = (void**)malloc(sizeof(void*) * getNumFileds(class));
	
	return res;
}

//nao faz free a classe
void freeObject(Object* obj)
{
	free(obj->fields);
	free(obj);
}

Class* getClassObj(Object* obj)
{
	return obj->class;
}

Type* getParamType(Object* obj)
{
	return obj->paramType;
}

void setParamType(Object* obj, Type* t)
{
	obj->paramType = t;
}

void* get(Object* obj, char* id)
{
	return obj->fields[getFieldIndex(obj->class, id)];
}

void set(Object* obj, char* id, void* val)
{
	obj->fields[getFieldIndex(obj->class, id)] = val;
}

FieldsIter* newFieldsIter(Object* obj)
{
	FieldsIter* res = (FieldsIter*)malloc(sizeof(FieldsIter));
	res->obj = obj;
	res->currPos = 0;

	return res;
}

void freeFieldsIter(FieldsIter* iter)
{
	free(iter);
}

int hasNextField(FieldsIter* iter)
{
	return iter->currPos < iter->obj->class->numFields;
}

void* nextField(FieldsIter* iter)
{
	return iter->obj->fields[iter->currPos++];
}
