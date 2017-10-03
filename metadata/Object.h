#ifndef _H_OBJECT
#define _H_OBJECT

#include "Class.h"

typedef struct
{
	Class* class;
	Type* paramType;
	void** fields;
} Object;

typedef struct
{
	Object* obj;
	int currPos;
} FieldsIter;

Object* newObject(Class* class);
void freeObject(Object* obj);

Class* getClassObj(Object* obj);
Type* getParamType(Object* obj);
void setParamType(Object* obj, Type* t);
void* get(Object* obj, char* id);
void set(Object* obj, char* id, void* val);

FieldsIter* newFieldsIter(Object* obj);
void freeFieldsIter(FieldsIter* iter);
int hasNextField(FieldsIter* iter);
void* nextField(FieldsIter* iter);

#endif
