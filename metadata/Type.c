#include "Type.h"
#include <stdlib.h>
#include "string.h"

Type* newType(int typeCode)
{
	Type* res = (Type*)malloc(sizeof(Type));
	res->typeCode = typeCode;
	res->className = NULL;
	res->elemType = NULL;
	
	return res;
}

void freeAllType(Type* type)
{
	free(type->className);
	free(type->elemType);
	free(type);
}

int equalType(Type* t1, Type* t2)
{
	if(t1->typeCode != t2->typeCode)
		return 0;
	
	if(t1->typeCode == T_CLASS && strcmp(t1->className, t2->className) != 0)
		return 0;
		
	if(t1->typeCode == T_ARRAY || t1->typeCode == T_THREAD)
		if(!equalType(t1->elemType, t2->elemType))
			return 0;
	
	return 1;
}

int getTypeCode(Type* type)
{
	return type->typeCode;
}

char* getClassName(Type* type)
{
	return type->className;
}

Type* getElemType(Type* type)
{
	return type->elemType;
}

void setClassName(Type* type, char* className)
{
	type->className = className;
}

void setElemType(Type* type, Type* elemType)
{
	type->elemType = elemType;
}
