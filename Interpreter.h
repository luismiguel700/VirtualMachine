#ifndef _H_INTERPRETER
#define _H_INTERPRETER

#include "./metadata/Thread.h"
#include "Runtime.h"
#include <stdio.h>

#define OK 0
#define SYNTAXE_ERROR 1
#define TYPE_ERROR 2

int execFile(FILE* file, char* classpath, int timeGC);

void* execMethod(Thread* thread, Runtime* runtime);

#endif
