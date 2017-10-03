#ifndef _H_FIELD
#define _H_FIELD

#include <stdlib.h>
#include "Type.h"

typedef struct
{
	Type* type;
	char* name;
} Field;

Field* newField(Type* type, char* name);
void freeAllField(Field* field);

Type* getType(Field* field);
char* getName(Field* field);

#endif
