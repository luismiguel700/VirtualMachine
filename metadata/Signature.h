#ifndef _H_SIGNATURE
#define _H_SIGNATURE

#include "Type.h"

typedef struct
{
	Type* retType;
	char* id;
	Type** argTypes;
	int numArgTypes;
} Signature;

Signature* newSignature(Type* retType, char* id);
void freeAllSignature(Signature* sig);
int equalSignature(Signature* sig1, Signature* sig2);
int equalSigParamType(Signature* sig1, Signature* sig2, char* id, Type* t);

Type* getRetType(Signature* sig);
char* getId(Signature* sig);
void setId(Signature* sig, char* id);
void addArgType(Signature* sig, Type* type);
Type* getArgType(Signature* sig, int i);
int getNumArgTypes(Signature* sig);

#endif
