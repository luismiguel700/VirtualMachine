#ifndef _H_TYPE
#define _H_TYPE

#define T_INT 0
#define T_CHAR 1
#define T_BOOL 2
#define T_THREAD 3
#define T_LOCK 4
#define T_ARRAY 5
#define T_CLASS 6

typedef struct type
{
	int typeCode;
	char* className;
	struct type* elemType;
} Type;

Type* newType(int typeCode);
void freeAllType(Type* type);
int equalType(Type* t1, Type* t2);

int getTypeCode(Type* type);
char* getClassName(Type* type);
Type* getElemType(Type* type);

void setClassName(Type* type, char* className);
void setElemType(Type* type, Type* elemType);

#endif
