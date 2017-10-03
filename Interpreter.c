#include "assembler/lex.yy.h"
#include "metadata/Class.h"
#include "metadata/Method.h"
#include "util/Stack.h"
#include "Runtime.h"
#include "metadata/Instructions.h"
#include "metadata/Type.h"
#include "metadata/Thread.h"
#include "metadata/Monitor.h"
#include "metadata/Array.h"
#include "metadata/Object.h"
#include "Interpreter.h"
#include "TypeSystem.h"
#include <pthread.h>

void fork2(Thread* thread, Runtime* runtime);
void wait2(Thread* thread, Runtime* runtime);
void shared(Thread* thread, Runtime* runtime);
void sync(Thread* thread, Runtime* runtime);
void newlock(Thread* thread, Runtime* runtime);
void ldloc(Thread* thread, Runtime* runtime);
void stloc(Thread* thread, Runtime* runtime);
void ldarg(Thread* thread, Runtime* runtime);
void pop2(Thread* thread, Runtime* runtime);
void nop(Thread* thread, Runtime* runtime);
void br(Thread* thread, Runtime* runtime);
void brfalse(Thread* thread, Runtime* runtime);
void ldint(Thread* thread, Runtime* runtime);
void ldchar(Thread* thread, Runtime* runtime);
void add(Thread* thread, Runtime* runtime);
void sub(Thread* thread, Runtime* runtime);
void mul(Thread* thread, Runtime* runtime);
void div2(Thread* thread, Runtime* runtime);
void mod(Thread* thread, Runtime* runtime);
void ceq(Thread* thread, Runtime* runtime);
void cgt(Thread* thread, Runtime* runtime);
void clt(Thread* thread, Runtime* runtime);
void and(Thread* thread, Runtime* runtime);
void or(Thread* thread, Runtime* runtime);
void not(Thread* thread, Runtime* runtime);
void newarr(Thread* thread, Runtime* runtime);
void ldlen(Thread* thread, Runtime* runtime);
void ldelem(Thread* thread, Runtime* runtime);
void stelem(Thread* thread, Runtime* runtime);
void newobj(Thread* thread, Runtime* runtime);
void call(Thread* thread, Runtime* runtime);
void ldfld(Thread* thread, Runtime* runtime);
void stfld(Thread* thread, Runtime* runtime);
void printint(Thread* thread, Runtime* runtime);
void printchar(Thread* thread, Runtime* runtime);
void printbool(Thread* thread, Runtime* runtime);
void dup(Thread* thread, Runtime* runtime);
void ret(Thread* thread, Runtime* runtime);

Thread* startFirstThread(Class* mainClass, Method* mainMethod, Runtime* runtime);
pthread_t* startGCThread(Runtime* runtime, int timeGC);
void* cleanHeap(void* args);

typedef struct
{
	Runtime* runtime;
	int timeGC;
} GCArgs;

const void (*eval[39]) (Thread* thread, Runtime* runtime) =
{
	ldint, ldchar, ldint, add, sub,
	mul, div2, ldarg, ldloc, stloc,
	fork2, wait2, shared, sync, newlock,
	pop2, nop, brfalse, br, mod,
	ceq, cgt, clt, and, or,
	not, newarr, ldlen, ldelem, stelem,
	newobj, call, ldfld, stfld, printint,
	printchar, printbool, dup, ret
};

int execFile(FILE* file, char* classpath, int timeGC)
{
	Class* mainClass = parse(file);

	if(mainClass == NULL)
		return SYNTAXE_ERROR;

	Method* mainMethod = getMethod(mainClass, newSignature(T_INT, "main"));

	if(mainMethod == NULL)
		return SYNTAXE_ERROR;

	Runtime* runtime = newRuntime(mainClass);
	parseClasspath(runtime, classpath);

	if(!checkProgram(runtime))
		return TYPE_ERROR;

	Thread* firstThread = startFirstThread(mainClass, mainMethod, runtime);
//	pthread_t* GCThread = startGCThread(runtime, timeGC);
	join(firstThread);
//	pthread_cancel(*GCThread); //apenas e enviado um sinal (para thread ser cancelada)
//	pthread_join(*GCThread, NULL); //antes de destruir o runtime, confirmar que a thread "terminou mesmo"
	freeAllRuntime(runtime);

	return OK;
}

void* execMethod(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	char byte = getByte(getCurrBlock(frame), nextIp(frame));

	while(byte != C_RET)
	{
		(*eval[(int)byte])(thread, runtime);
		byte = getByte(getCurrBlock(frame), nextIp(frame));
	}

	return pop(getStack(frame));
}

Thread* startFirstThread(Class* mainClass, Method* mainMethod, Runtime* runtime)
{
	Thread* thread = newThread();
	Stack* stack = newStack(1);
	push(stack, (void*)newObject(mainClass));
	Frame* frame = newFrame(stack, getSignature(mainMethod));
	addFrame(thread, frame);
	launch(thread, (struct runtime*)runtime);

	return thread;
}

pthread_t* startGCThread(Runtime* runtime, int timeGC)
{
	pthread_t* thread = (pthread_t*)malloc(sizeof(pthread_t));
	GCArgs* args = (GCArgs*)malloc(sizeof(GCArgs));
	args->runtime = runtime;
	args->timeGC = timeGC;
	pthread_create(thread, NULL, cleanHeap, (void*)args);

	return thread;
}

void* cleanHeap(void* args)
{
	GCArgs* argsGC = (GCArgs*)args;
	Runtime* runtime = argsGC->runtime;
	int timeGC = argsGC->timeGC;

	while(1) //a thread principal termina a thread que executa o GC
	{
		startShared(runtime->threadsMon);
		startSync(runtime->heapMon);
		doGC(runtime->heap, runtime->threads, runtime->numThreads);
		endSync(runtime->heapMon);
		endShared(runtime->threadsMon);

		sleep(timeGC/1000);
	}

	return NULL;
}

void fork2(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int constIndex = getByte(getCurrBlock(frame), nextIp(frame));
	Signature* sig = (Signature*)getConstant(frameGetClass(frame), constIndex);

	Frame* firstFrame = newFrame(stack, sig);
	Thread* childThread = newThread();
	addFrame(childThread, firstFrame);
	launch(childThread, (struct runtime*)runtime);
	push(stack, (void*)childThread);
}

void wait2(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	void* val = join((Thread*)pop(stack));
	push(stack, val);
}

void shared(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int constIndex = getByte(getCurrBlock(frame), nextIp(frame));
	Signature* sig = (Signature*)getConstant(frameGetClass(frame), constIndex);

	Frame* childFrame = newFrame(stack, sig);
	addFrame(thread, childFrame);
	Monitor* mon = (Monitor*)pop(stack);
	startShared(mon);
	void* res = execMethod(thread, runtime);
	endShared(mon);
	removeFrame(thread);
	push(getStack(frame), res);
}

void sync(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int constIndex = getByte(getCurrBlock(frame), nextIp(frame));
	Signature* sig = (Signature*)getConstant(frameGetClass(frame), constIndex);

	Frame* childFrame = newFrame(stack, sig);
	addFrame(thread, childFrame);
	Monitor* mon = (Monitor*)pop(stack);
	startSync(mon);
	void* res = execMethod(thread, runtime);
	endSync(mon);
	removeFrame(thread);
	push(getStack(frame), res);
}

void newlock(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	startSync(runtime->heapMon);
	Monitor* mon = addMon(runtime->heap);
	endSync(runtime->heapMon);
	push(stack, (void*)mon);
}

void ldloc(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int index = (int)getByte(getCurrBlock(frame), nextIp(frame));
	push(stack, getVar(frame, index));
}

void stloc(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int index = (int)getByte(getCurrBlock(frame), nextIp(frame));
	setVar(frame, index, pop(stack));
}

void ldarg(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int index = (int)getByte(getCurrBlock(frame), nextIp(frame));
	push(stack, getArg(frame, index));
}

void pop2(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	pop(stack);
}

void nop(Thread* thread, Runtime* runtime) {}

void br(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	int index = (int)getByte(getCurrBlock(frame), nextIp(frame));
	char* label = (char*)getConstant(frameGetClass(frame), index);
	setCurrBlock(frame, label);
}

void brfalse(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int index = (int)getByte(getCurrBlock(frame), nextIp(frame));
	char* label = (char*)getConstant(frameGetClass(frame), index);

	if(!(int)pop(stack))
		setCurrBlock(frame, label);
}

void ldint(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int i = (int)getByte(getCurrBlock(frame), nextIp(frame));
	push(stack, (void*)i);
}

void ldchar(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	char c = getByte(getCurrBlock(frame), nextIp(frame));
	push(stack, (void*)c);
}

void add(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) + (int)pop(stack);
	push(stack, (void*)res);
}

void sub(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int r = (int)pop(stack);
	int l = (int)pop(stack);
	push(stack, (void*)(l-r));
}

void mul(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) * (int)pop(stack);
	push(stack, (void*)res);
}

void div2(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int r = (int)pop(stack);
	int l = (int)pop(stack);
	push(stack, (void*)(l/r));
}

void mod(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int r = (int)pop(stack);
	int l = (int)pop(stack);
	push(stack, (void*)(l%r));
}

void ceq(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) == (int)pop(stack);
	push(stack, (void*)res);
}

void cgt(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) < (int)pop(stack);
	push(stack, (void*)res);
}

void clt(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) > (int)pop(stack);
	push(stack, (void*)res);
}

void and(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) && (int)pop(stack);
	push(stack, (void*)res);
}

void or(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = (int)pop(stack) || (int)pop(stack);
	push(stack, (void*)res);
}

void not(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int res = !(int)pop(stack);
	push(stack, (void*)res);
}

void newarr(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int size = (int)pop(stack);
	int constIndex = (int)getByte(getCurrBlock(frame), nextIp(frame));
	Type* type = (Type*)getConstant(frameGetClass(frame), constIndex);
	startSync(runtime->heapMon);
	Array* array = addArray(runtime->heap, size, type);
	endSync(runtime->heapMon);
	push(stack, (void*)array);
}

void ldlen(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	Array* array = (Array*)pop(stack);
	push(stack, (void*)size(array));
}

void ldelem(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	int index = (int)pop(stack);
	Array* array = (Array*)pop(stack);

	if(index >= size(array))
		return; //ERRO

	push(stack, getElem(array, index));
}

void stelem(Thread* thread, Runtime* runtime)
{
	Stack* stack = getStack(getFrame(thread));
	void* val = pop(stack);
	int index = (int)pop(stack);
	Array* array = (Array*)pop(stack);

	if(index >= size(array))
		return; //ERRO

	setElem(array, index, val);
}

void newobj(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int constIndex = getByte(getCurrBlock(frame), nextIp(frame));
	char* className = (char*)getConstant(frameGetClass(frame), constIndex);
	constIndex = getByte(getCurrBlock(frame), nextIp(frame));
	Type* paramType = (Type*)getConstant(frameGetClass(frame), constIndex);

	startSync(runtime->heapMon);
	Object* obj = addObj(runtime->heap, getClass(runtime, className));
	setParamType(obj, paramType);
	endSync(runtime->heapMon);
	push(stack, obj);
}

void call(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int constIndex = getByte(getCurrBlock(frame), nextIp(frame));
	Signature* sig = (Signature*)getConstant(frameGetClass(frame), constIndex);

	Frame* childFrame = newFrame(stack, sig);
	addFrame(thread, childFrame);
	void* res = execMethod(thread, runtime);
	removeFrame(thread);
	push(getStack(frame), res);
}

void ldfld(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	Object* obj = (Object*)pop(stack);
	int constIndex = (int)getByte(getCurrBlock(frame), nextIp(frame));
	Class* class = frameGetClass(frame);
	push(stack, get(obj, getConstant(class, constIndex)));
}

void stfld(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	void* val = pop(stack);
	Object* obj = (Object*)pop(stack);
	int constIndex = (int)getByte(getCurrBlock(frame), nextIp(frame));
	Class* class = frameGetClass(frame); 
	set(obj, getConstant(class, constIndex), val);
}

void printint(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	printf("%d", (int)pop(stack));
}

void printchar(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	printf("%c", (char)pop(stack));
}

void printbool(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	int bool = (int)pop(stack);

	if(bool)
		printf("true");
	else
		printf("false");
}

void dup(Thread* thread, Runtime* runtime)
{
	Frame* frame = getFrame(thread);
	Stack* stack = getStack(frame);
	push(stack, peek(stack));
}

void ret(Thread* thread, Runtime* runtime) {}
