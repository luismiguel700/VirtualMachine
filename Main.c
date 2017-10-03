#include <stdio.h>
#include "Interpreter.h"

int main(int argc, char** argv)
{
	if(argc < 4)
	{
		printf("Wrong number of arguments (file, classpath and timeGC)\n");
		return -1;
	}
	
	FILE* file = fopen(argv[1], "r");
	
	if(file == NULL)
	{
		printf("Invalid file\n");
		return -1;
	}

	int res = execFile(file, argv[2], atoi(argv[3]));
	
	switch(res)
	{
		case SYNTAXE_ERROR:
			printf("Syntaxe Error\n");
			break;
		case TYPE_ERROR:
			printf("Type Error\n");
			break;
	}

	fclose(file);
	
	return 0;
}
