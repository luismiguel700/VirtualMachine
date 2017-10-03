#include <stdlib.h>
#include "Signature.h"
#include "string.h"

#define SIZE 100

Signature* newSignature(Type* retType, char* id)
{
	Signature* res = (Signature*)malloc(sizeof(Signature));
	res->retType = retType;
	res->id = id;
	res->argTypes = (Type**)malloc(sizeof(Type*) * SIZE);
	res->numArgTypes = 0;
	
	return res;
}

void freeAllSignature(Signature* sig)
{
	free(sig->retType);
	free(sig->id);
	int i;
	
	for(i=0; i<sig->numArgTypes; i++)
		free(sig->argTypes[i]);
	
	free(sig->argTypes);
	free(sig);
}

//nao compara o tipo de retorno
int equalSignature(Signature* sig1, Signature* sig2)
{
	if(strcmp(sig1->id, sig2->id) != 0)
		return 0;
	
	if(sig1->numArgTypes != sig2->numArgTypes)
		return 0;

	int i;

	for(i=0; i < sig1->numArgTypes; i++)
		if(!equalType(sig1->argTypes[i], sig2->argTypes[i]))
			return 0;

	return 1;
}

//nao compara o tipo de retorno e tem em conta os tipos parametricos
int equalSigParamType(Signature* sig1, Signature* sig2, char* id, Type* paramT)
{
	if(strcmp(sig1->id, sig2->id) != 0)
		return 0;

	if(sig1->numArgTypes != sig2->numArgTypes)
		return 0;

	int i;

	for(i=0; i < sig1->numArgTypes; i++)
	{
		Type* t2 = sig2->argTypes[i];

		if(t2->typeCode == T_CLASS && strcmp(getClassName(t2), id) == 0)
		{
			if(!equalType(sig1->argTypes[i], paramT))
				return 0;
		}
		else
		{
			if(!equalType(sig1->argTypes[i], sig2->argTypes[i]))
				return 0;
		}
	}

	return 1;
}

void setId(Signature* sig, char* id)
{
	sig->id = id;
}

Type* getRetType(Signature* sig)
{
	return sig->retType;
}

char* getId(Signature* sig)
{
	return sig->id;
}

void addArgType(Signature* sig, Type* type)
{
	sig->argTypes[sig->numArgTypes] = type;
	sig->numArgTypes++;
}

Type* getArgType(Signature* sig, int i)
{
	return sig->argTypes[i];
}

int getNumArgTypes(Signature* sig)
{
	return sig->numArgTypes;
}
