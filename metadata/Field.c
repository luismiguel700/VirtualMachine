#include <stdlib.h>
#include "Field.h"

Field* newField(Type* type, char* name)
{
	Field* res = (Field*)malloc(sizeof(Field));
	res->type = type;
	res->name = name;
	
	return res;
}

void freeAllField(Field* field)
{
	free(field->type);
	free(field->name);
	free(field);
}

Type* getType(Field* field)
{
	return field->type;
}

char* getName(Field* field)
{
	return field->name;
}
