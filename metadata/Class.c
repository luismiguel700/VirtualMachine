#include <stdlib.h>
#include "Class.h"
#include "string.h"

#define SIZE 100

Class* newClass(char* name)
{
	Class* res = (Class*)malloc(sizeof(Class));
	res->name = name;
	res->constants = (void**)malloc(sizeof(void*) *  SIZE);
	res->fields = (Field**)malloc(sizeof(Field*) * SIZE);	
	res->methods = (Method**)malloc(sizeof(Method*) * SIZE);
	res->numConstants = 0;
	res->numFields = 0;
	res->numMethods = 0;
	res->paramId = NULL;
	
	return res;
}

void freeAllClass(Class* class)
{
	free(class->name);
	int i;
	
	for(i=0; i<class->numConstants; i++)
		free(class->constants[i]);

	free(class->constants);

	for(i=0; i<class->numFields; i++)
		free(class->fields[i]);
		
	free(class->fields);
		
	for(i=0; i<class->numMethods; i++)
		free(class->methods[i]);
		
	free(class->methods);
	free(class);
}

void* getConstant(Class* class, int i)
{
	return class->constants[i];
}

char* getClassId(Class* class)
{
	return class->name;
}

char* getParamId(Class* class)
{
	return class->paramId;
}

Field* getField(Class* class, char* field)
{
	int i;
	
	for(i=0; i<class->numFields; i++)
		if( strcmp(getName(class->fields[i]), field) == 0 )
			return class->fields[i];

	return NULL;
}

int getFieldIndex(Class* class, char* field)
{
	int i;
	
	for(i=0; i<class->numFields; i++)
		if( strcmp(getName(class->fields[i]), field) == 0 )
			return i;

	return -1;	
}

int getNumFileds(Class* class)
{
	return class->numFields;
}

Method* getMethod(Class* class, Signature* sig)
{
	int i;
	
	for(i=0; i<class->numMethods; i++)
		if( equalSignature(getSignature(class->methods[i]), sig) )
			return class->methods[i];
	
	return NULL;
}

Method* getMethodParamType(Class* class, Signature* sig, Type* paramType)
{
	if(class->paramId == NULL)
		return getMethod(class, sig);

	int i;

	for(i=0; i<class->numMethods; i++)
	{
		Signature* sig2 = getSignature(class->methods[i]);

		if( equalSigParamType(sig, sig2, class->paramId, paramType) )
			return class->methods[i];
	}

	return NULL;
}

void setParamId(Class* class, char* id)
{
	class->paramId = id;
}

int addConstant(Class* class, void* constant)
{
	class->constants[class->numConstants] = constant;

	return class->numConstants++;
}

void addField(Class* class, Field* field)
{		
	class->fields[class->numFields] = field;
	class->numFields++;
}

void addMethod(Class* class, Method* method)
{
	class->methods[class->numMethods] = method;
	class->numMethods++;
}

int isParamType(Class* c, Type* t)
{
	if(c->paramId != NULL && t->typeCode == T_CLASS)
	{
		if(strcmp(c->paramId, getClassName(t)) == 0)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

FieldsTypesIter* newFieldsTypesIter(Class* class)
{
	FieldsTypesIter* res = (FieldsTypesIter*)malloc(sizeof(FieldsTypesIter));
	res->class = class;
	res->index = 0;

	return res;
}

void freeFieldsTypesIter(FieldsTypesIter* iter)
{
	free(iter);
}

Type* nextFieldType(FieldsTypesIter* iter)
{
	return getType(iter->class->fields[iter->index++]);
}

int hasNextFieldType(FieldsTypesIter* iter)
{
	return iter->index < iter->class->numFields;
}

MethodIter* newMethodIter(Class* class)
{
	MethodIter* res = (MethodIter*)malloc(sizeof(MethodIter));
	res->class = class;
	res->index = 0;

	return res;
}

void freeMethodIter(MethodIter* iter)
{
	free(iter);
}

Method* nextMethod(MethodIter* iter)
{
	return iter->class->methods[iter->index++];
}

int hasNextMethod(MethodIter* iter)
{
	return iter->index < iter->class->numMethods;
}
