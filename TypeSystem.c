#include "TypeSystem.h"
#include "Runtime.h"
#include "metadata/Class.h"
#include "metadata/Method.h"
#include "metadata/Signature.h"
#include "metadata/Block.h"
#include "metadata/Instructions.h"

#include <string.h>

int checkLdint(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdchar(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdbool(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkArith(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkComp(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdarg(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdloc(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkStloc(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkFork2(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkWait2(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkSharedSync(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkNewlock(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkPop2(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkNop(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkBrfalse(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkBr(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkBoolOp(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkNot(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkNewarr(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdlen(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdelem(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkStelem(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkNewobj(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkCall(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkLdfld(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkStfld(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkPrintint(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkPrintchar(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkPrintbool(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkDup(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);
int checkRet(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s);

int checkClass(Runtime* runtime, Class* class);
int checkMethod(Runtime* runtime, Class* class, Method* method);
int checkBlock(Runtime* runtime, Class* class, Method* method, Block* block);
int checkArgsAndObj(Runtime* r, Class* c, Method* m, Block* b, Signature* sig, Stack* s);

const int (*check[39])(Runtime*, Class*, Method*, Block*, int* ip, Stack*) =
{
	checkLdint, checkLdchar, checkLdbool, checkArith, checkArith,
	checkArith, checkArith, checkLdarg, checkLdloc, checkStloc,
	checkFork2, checkWait2, checkSharedSync, checkSharedSync, checkNewlock,
	checkPop2, checkNop, checkBrfalse, checkBr, checkArith,
	checkComp, checkComp, checkComp, checkBoolOp, checkBoolOp,
	checkNot, checkNewarr, checkLdlen, checkLdelem, checkStelem,
	checkNewobj, checkCall, checkLdfld, checkStfld, checkPrintint,
	checkPrintchar, checkPrintbool, checkDup, checkRet
};

//depois de introduzir os bools na ling de alto n’vel, alterar algumas funcoes (not, ceq, brfalse, ...)

int checkProgram(Runtime* runtime)
{
	ClassIter* iter = newClassIter(runtime);
	int res = 1;

	while(hasNextClass(iter) && res)
		res = checkClass(runtime, nextClass(iter));

	return res;
}

int checkClass(Runtime* runtime, Class* class)
{
	MethodIter* iter = newMethodIter(class);
	int res = 1;

	while(hasNextMethod(iter) && res)
		res = checkMethod(runtime, class, nextMethod(iter));

	return res;
}

int checkMethod(Runtime* runtime, Class* class, Method* method)
{
	BlockIter* iter = newBlockIter(method);
	int res = 1;

	while(hasNextBlock(iter) && res)
		res = checkBlock(runtime, class, method, nextBlock(iter));

	return res;
}

int checkBlock(Runtime* runtime, Class* class, Method* method, Block* block)
{
	int* ip = (int*)malloc(sizeof(int));
	Stack* stackType = cloneStack(getStackType(block));
	char byte;
	int res;

	do
	{
		byte = getByte(block, (*ip)++);
//		printf("%s\t%s\n", getId(getSignature(method)), getLabel(block));
		res = (*check[(int)byte])(runtime, class, method, block, ip, stackType);
	}
	while(byte != C_BR && byte != C_RET && res);

	return res;
}

int checkArgsAndObj(Runtime* r, Class* c, Method* m, Block* b, Signature* sig, Stack* s)
{
	int numArgs = getNumArgTypes(sig);

	if(getNumber(s) < numArgs+1)
		return 0;

	int i;

	for(i=numArgs-1; i>=0; i--)
	{
		Type* argType = (Type*)pop(s);

		if(!equalType(argType, getArgType(sig, i)))
			return 0;
	}

	Type* t = (Type*)peek(s);

	if(t->typeCode != T_CLASS)
		return 0;

	Class* objClass = getClass(r, getClassName(t));

	return getMethodParamType(objClass, sig, getElemType(t)) != NULL; //verifica se a assinatura existe
}

int checkLdint(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	push(s, (void*)newType(T_INT));
	(*ip)++;

	return 1;
}

int checkLdchar(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	push(s, (void*)newType(T_CHAR));
	(*ip)++;

	return 1;
}

int checkLdbool(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	push(s, (void*)newType(T_BOOL));
	(*ip)++;

	return 1;
}

int checkArith(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 2)
		return 0;

	Type* t1 = (Type*)pop(s);
	Type* t2 = (Type*)peek(s);

	return t1->typeCode == T_INT && t2->typeCode == T_INT;
}

int checkComp(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 2)
		return 0;

	Type* t1 = (Type*)pop(s);
	Type* t2 = (Type*)pop(s);

	if(t1->typeCode != T_INT || t2->typeCode != T_INT)
		return 0;

	push(s, (void*)newType(T_INT));

	return 1;
}

int checkLdarg(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);
	Signature* sig = getSignature(m);

	if(index < 0 || index > getNumArgTypes(sig))
		return 0;

	if(index == 0)
	{
		Type* t = newType(T_CLASS);
		setClassName(t, getClassId(c));
		Type* paramType = newType(T_CLASS);
		setClassName(paramType, getParamId(c));
		setElemType(t, paramType);
		push(s, t);
	}
	else
		push(s, (void*)getArgType(sig, index-1));

	return 1;
}

int checkLdloc(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);

	if(index < 0 || index >= getNumVarTypes(m))
		return 0;

	push(s, (void*)getVarType(m, index));

	return 1;
}

int checkStloc(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);

	if(index < 0 || index >= getNumVarTypes(m))
		return 0;

	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	return equalType(t, getVarType(m, index));
}

int checkFork2(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);
	Signature* sig = (Signature*)getConstant(c, index);

	if(!checkArgsAndObj(r, c, m, b, sig, s))
		return 0;

	Type* tObj = (Type*)pop(s);
	Class* c2 = getClass(r, getClassName(tObj));
	Type* threadType = newType(T_THREAD);

	if(isParamType(c2, getRetType(sig)))
		setElemType(threadType, getElemType(tObj));
	else
		setElemType(threadType, getRetType(sig));

	push(s, (void*)threadType);

	return 1;
}

int checkWait2(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	if(t->typeCode != T_THREAD)
		return 0;

	push(s, (void*)getElemType(t));

	return 1;
}

int checkSharedSync(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);
	Signature* sig = (Signature*)getConstant(c, index);

	if(!checkArgsAndObj(r, c, m, b, sig, s))
		return 0;

	Type* tObj = (Type*)pop(s);
	Class* c2 = getClass(r, getClassName(tObj));
	Type* t = (Type*)pop(s);

	if(t->typeCode != T_LOCK)
		return 0;

	if(isParamType(c2, getRetType(sig)))
		push(s, (void*)getElemType(tObj));
	else
		push(s, (void*)getRetType(sig));

	return 1;
}

int checkNewlock(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	push(s, newType(T_LOCK));

	return 1;
}

int checkPop2(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	pop(s);

	return 1;
}

int checkNop(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	return 1;
}

int checkBrfalse(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	if(t->typeCode != T_INT)
		return 0;

	return checkBr(r, c, m, b, ip, s);
}

int checkBr(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);
	char* label = (char*)getConstant(c, index);
	Block* b2 = getBlock(m, label);

	if(b2 == NULL)
		return 0;

	return equalStack(s, getStackType(b2), (int (*)(void*, void*))equalType);
}

int checkBoolOp(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 2)
		return 0;

	Type* t1 = (Type*)pop(s);
	Type* t2 = (Type*)peek(s);

	return t1->typeCode == T_BOOL && t2->typeCode == T_BOOL;
}

int checkNot(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)peek(s);

	return t->typeCode == T_INT;
}

int checkNewarr(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	if(t->typeCode != T_INT)
		return 0;

	int index = (int)getByte(b, (*ip)++);
	Type* typeElem = (Type*)getConstant(c, index);
	Type* typeArray = newType(T_ARRAY);
	setElemType(typeArray, typeElem);
	push(s, (void*)typeArray);

	return 1;
}

int checkLdlen(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	if(t->typeCode != T_ARRAY)
		return 0;

	push(s, newType(T_INT));

	return 1;
}

int checkLdelem(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 2)
		return 0;

	Type* t1 = (Type*)pop(s);
	Type* t2 = (Type*)pop(s);

	if(t1->typeCode != T_INT || t2->typeCode != T_ARRAY)
		return 0;

	push(s, (void*)getElemType(t2));

	return 1;
}

int checkStelem(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 3)
		return 0;

	Type* t1 = (Type*)pop(s);
	Type* t2 = (Type*)pop(s);
	Type* t3 = (Type*)pop(s);

	if(t2->typeCode != T_INT || t3->typeCode != T_ARRAY)
		return 0;

	if(t1->typeCode != getElemType(t3)->typeCode)
		return 0;

	return 1;
}

int checkNewobj(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	char* name = (char*)getConstant(c, getByte(b, (*ip)++));
	Class* objClass = getClass(r, name);

	if(objClass == NULL)
		return 0;

	Type* paramType = (Type*)getConstant(c, getByte(b, (*ip)++));

	if(paramType == NULL && getParamId(objClass) != NULL)
		return 0;
	else if(paramType != NULL && getParamId(objClass) == NULL)
		return 0;

	Type* objType = newType(T_CLASS);
	setClassName(objType, name);
	setElemType(objType, paramType);
	push(s, objType);

	return 1;
}

int checkCall(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	int index = (int)getByte(b, (*ip)++);
	Signature* sig = (Signature*)getConstant(c, index);

	if(!checkArgsAndObj(r, c, m, b, sig, s))
		return 0;

	Type* tObj = (Type*)pop(s);
	Class* c2 = getClass(r, getClassName(tObj));

	if(isParamType(c2, getRetType(sig)))
		push(s, (void*)getElemType(tObj));
	else
		push(s, (void*)getRetType(sig));

	return 1;
}

int checkLdfld(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	if(t->typeCode != T_CLASS)
		return 0;

	int index = getByte(b, (*ip)++);
	Class* c2 = getClass(r, getClassName(t));
	Field* f = getField(c2, (char*)getConstant(c, index));

	if(f == NULL)
		return 0;

	if(isParamType(c2, getType(f)))
		push(s, (void*)getElemType(t));
	else
		push(s, (void*)getType(f));

	return 1;
}

int checkStfld(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 2)
		return 0;

	Type* t1 = (Type*)pop(s);
	Type* t2 = (Type*)pop(s);

	if(t2->typeCode != T_CLASS)
		return 0;

	int index = getByte(b, (*ip)++);
	Class* c2 = getClass(r, getClassName(t2));
	Field* f = getField(c2, (char*)getConstant(c, index));

	if(f == NULL)
		return 0;

	if(isParamType(c2, getType(f)))
		return equalType(t1, getElemType(t2));
	else
		return equalType(t1, getType(f));
}

int checkPrintint(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	return t->typeCode == T_INT;
}

int checkPrintchar(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	return t->typeCode == T_CHAR;
}

int checkPrintbool(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	Type* t = (Type*)pop(s);

	return t->typeCode == T_BOOL;
}

int checkDup(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) < 1)
		return 0;

	push(s, peek(s));

	return 1;
}

int checkRet(Runtime* r, Class* c, Method* m, Block* b, int* ip, Stack* s)
{
	if(getNumber(s) != 1)
		return 0;

	Signature* sig = getSignature(m);

	return equalType((Type*)pop(s), getRetType(sig));
}
