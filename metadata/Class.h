#ifndef _H_CLASS
#define _H_CLASS

#include "Field.h"
#include "Method.h"

typedef struct
{
	char* name;
	char* paramId;
	void** constants;
	Field** fields;
	Method** methods;
	int numConstants;
	int numFields;
	int numMethods;
} Class;

typedef struct
{
	Class* class;
	int index;
} FieldsTypesIter;

typedef struct
{
	Class* class;
	int index;
} MethodIter;

Class* newClass(char* name);
void freeAllClass(Class* class);

char* getClassId(Class* class);
char* getParamId(Class* class);
void* getConstant(Class* class, int index);
Field* getField(Class* class, char* field);
int getFieldIndex(Class* class, char* field);
int getNumFileds(Class* class);
Method* getMethod(Class* class, Signature* sig);
Method* getMethodParamType(Class* class, Signature* sig, Type* paramType);

void setParamId(Class* class, char* id);
int addConstant(Class* class, void* constant);
void addField(Class* class, Field* field);
void addMethod(Class* class, Method* method);

int isParamType(Class* c, Type* t);

FieldsTypesIter* newFieldsTypesIter(Class* class);
void freeFieldsTypesIter(FieldsTypesIter* iter);
Type* nextFieldType(FieldsTypesIter* iter);
int hasNextFieldType(FieldsTypesIter* iter);

MethodIter* newMethodIter(Class* class);
void freeMethodIter(MethodIter* iter);
Method* nextMethod(MethodIter* iter);
int hasNextMethod(MethodIter* iter);

#endif
