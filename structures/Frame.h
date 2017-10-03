/* ALTERAR */
#include "../metadata/Block.h"
#include "../metadata/Method.h"
#include "../metadata/Class.h"
#include "../util/Stack.h"

typedef struct
{
	Class* class;
	Method* method;
	Block* block;
	Stack* stack;
	void** vars;
	void** args;
	int numVars;
	int numArgs;
	int ip;
} Frame;

Frame* newFrame(Stack* stack, Signature* sig);
void freeFrame(Frame* frame);

Class* frameGetClass(Frame* frame);
Method* frameGetMethod(Frame* frame);
Stack* getStack(Frame* frame);

void setVar(Frame* frame, int i, void* var);
void* getVar(Frame* frame, int i);
int getNumVars(Frame* frame);

void addArg(Frame* frame, void* arg);
void* getArg(Frame* frame, int i);
int getNumArgs(Frame* frame);

Block* getCurrBlock(Frame* frame);
void setCurrBlock(Frame* frame, char* label);
int getIp(Frame* frame);
int nextIp(Frame* frame);
