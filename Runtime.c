#include "Runtime.h"
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include "assembler/lex.yy.h"

#define EXTENSION "tmvm"
#define SIZE 100

//mudar para hastables

Runtime* newRuntime(Class* mainClass)
{
	Runtime* res = (Runtime*)malloc(sizeof(Runtime));
	res->classes = (Class**)malloc(sizeof(Class*) * SIZE);
	res->classes[0] = mainClass;
	res->numClasses = 1;
	res->threads = (Thread**)malloc(sizeof(Thread*) * SIZE);
	res->numThreads = 0;
	res->threadsMon = newMonitor();
	res->heap = (struct heap*)newHeap();
	res->heapMon = newMonitor();

	return res;
}

void parseClasspath(Runtime* runtime, char* classpath)
{
	DIR* dir = opendir(classpath);
	struct dirent* ent = readdir(dir);

	while(ent != NULL)
	{
		char* ext = strchr(ent->d_name, (int)'.');

		if(ext != NULL && strcmp(ext+1, EXTENSION) == 0)
		{
			char* path = (char*)malloc(sizeof(char)*SIZE);
			strcpy(path, classpath);
			strcat(path, "/");
			FILE* file = fopen(strcat(path, ent->d_name), "r");

			if(file != NULL)
			{
				Class* c = parse(file);

				if(c != NULL && getClass(runtime, getClassId(c)) == NULL)
					addClass(runtime, c);
			}

			fclose(file);
		}

		ent = readdir(dir);
	}

	closedir(dir);
}

void freeAllRuntime(Runtime* runtime)
{
	int i;
	
	for(i=0; i<runtime->numClasses; i++)
		freeAllClass(runtime->classes[i]);

	free(runtime->classes);

	for(i=0; i<runtime->numThreads; i++)
		freeThread(runtime->threads[i]);

	free(runtime->threads);
	freeAllHeap((struct heap*)runtime->heap);
	free(runtime);
}

Class* getClass(Runtime* runtime, char* name)
{
	int i;
	
	for(i=0; i<runtime->numClasses; i++)
		if(strcmp(getClassId(runtime->classes[i]), name) == 0)
			return runtime->classes[i];
			
	return NULL;
}

Thread* getThread(Runtime* runtime, int tid)
{
	return runtime->threads[tid];
}

struct heap* getHeap(Runtime* runtime)
{
	return runtime->heap;
}

int getNumClass(Runtime* runtime)
{
	return runtime->numClasses;
}

int getNumThread(Runtime* runtime)
{
	return runtime->numThreads;
}

void addClass(Runtime* runtime, Class* class)
{
	runtime->classes[runtime->numClasses] = class;
	runtime->numClasses++;
}

void addThread(Runtime* runtime, Thread* thread)
{
	runtime->threads[runtime->numThreads] = thread;
	runtime->numThreads++;	
}

ClassIter* newClassIter(Runtime* runtime)
{
	ClassIter* res = (ClassIter*)malloc(sizeof(ClassIter));
	res->runtime = runtime;
	res->currPos = 0;

	return res;
}

void freeClassIter(ClassIter* iter)
{
	free(iter);
}

Class* nextClass(ClassIter* iter)
{
	return iter->runtime->classes[iter->currPos++];
}

int hasNextClass(ClassIter* iter)
{
	return iter->currPos < iter->runtime->numClasses;
}
