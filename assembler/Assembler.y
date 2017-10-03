//Nao esquecer: transformar logo as labels para indices (em vez de fazer isso no interpretador)

%{
#include <stdio.h>
#include "lex.yy.h"
#include "string.h"
#include "../metadata/Type.h"
#include "../metadata/Class.h"
#include "../metadata/Field.h"
#include "../metadata/Method.h"
#include "../metadata/Signature.h"
#include "../metadata/Block.h"
#include "../util/Stack.h"

#define SIZE 100

extern FILE *yyin;
void yyerror(char const *);
Class* parse(FILE* file);

Class* currClass;
Method* currMethod;
Signature* currSig;
Block* currBlock;
Stack* currStack;
%}

%start init

%type <type> type
%type <stack> stackType

%union 
{
	int integer;
	int bool;
	char byte;
	char* string;
	struct type* type;
	struct stack* stack;
}

%token CLASS 
%token FIELD 
%token METHOD
%token LOCALS
%token LPAR 
%token RPAR 
%token LPAR2 
%token RPAR2
%token LPAR3 
%token RPAR3
%token LPAR4 
%token RPAR4
%token COMMA 
%token COLON
%token SEMICOLON
%token EMPTY

%token <integer> TYPE_INT
%token <integer> TYPE_BOOL
%token <integer> TYPE_CHAR
%token <integer> TYPE_THREAD
%token <integer> TYPE_LOCK

%token <byte> LDINT
%token <byte> LDCHAR
%token <byte> LDBOOL
%token <byte> ADD
%token <byte> SUB

%token <byte> MUL
%token <byte> DIV
%token <byte> LDARG
%token <byte> LDLOC
%token <byte> STLOC

%token <byte> FORK
%token <byte> WAIT
%token <byte> SHARED
%token <byte> SYNC
%token <byte> NEWLOCK

%token <byte> POP
%token <byte> NOP
%token <byte> BRFALSE
%token <byte> BR
%token <byte> MOD

%token <byte> CEQ
%token <byte> CGT
%token <byte> CLT
%token <byte> AND
%token <byte> OR

%token <byte> NOT
%token <byte> NEWARR
%token <byte> LDLEN
%token <byte> LDELEM
%token <byte> STELEM

%token <byte> NEWOBJ
%token <byte> CALL
%token <byte> LDFLD
%token <byte> STFLD
%token <byte> PRINTINT

%token <byte> PRINTCHAR
%token <byte> PRINTBOOL
%token <byte> DUP
%token <byte> RET

%token <integer> INT
%token <integer> BOOL
%token <byte> CHAR
%token <string> ID




%%




init: CLASS ID { currClass = newClass($2); } LPAR2 fields methods RPAR2
|	CLASS ID LPAR4 ID RPAR4 { currClass = newClass($2); setParamId(currClass, $4); } LPAR2 fields methods RPAR2
;

fields: /*nothing*/
|	FIELD type ID SEMICOLON { addField(currClass, newField($2, $3)); } fields
;

methods: /*nothing*/
|	METHOD type ID { currSig = newSignature($2, $3); currMethod = newMethod(currSig); }
	LPAR methodArgs RPAR LPAR2 locals blocks RPAR2 { addMethod(currClass, currMethod); } methods
;

locals: /*nothing*/
|	LOCALS LPAR localTypes RPAR 
;

localTypes: /*nothing*/
|	type { addVarType(currMethod, $1); }
|	type COMMA { addVarType(currMethod, $1); } localTypes
;

blocks: /*nothing*/
|	ID COLON stackType { currBlock = newBlock($1, $3); } instructions endBlock { addBlock(currMethod, currBlock); } blocks
;

stackType: EMPTY { $$ = newStack(SIZE); }
|	LPAR2 { currStack = newStack(SIZE); } stackTypes RPAR2 { $$ = currStack; }
;

stackTypes: type { push(currStack, $1); }
|	type { push(currStack, $1); } COMMA stackTypes
;

instructions: /*nothing*/
|	instructionAndArgs instructions
;

instructionAndArgs: LDINT INT { addByte(currBlock, $1); addByte(currBlock, $2); }
|	LDCHAR CHAR { addByte(currBlock, $1); addByte(currBlock, $2); }
|	LDBOOL BOOL { addByte(currBlock, $1); addByte(currBlock, $2); }
|	ADD { addByte(currBlock, $1); }
|	SUB { addByte(currBlock, $1); }

|	MUL { addByte(currBlock, $1); }
|	DIV { addByte(currBlock, $1); }
|	LDARG INT { addByte(currBlock, $1); addByte(currBlock, $2); }
|	LDLOC INT { addByte(currBlock, $1); addByte(currBlock, $2); }
|	STLOC INT { addByte(currBlock, $1); addByte(currBlock, $2); }

|	FORK { addByte(currBlock, $1); } methodCall
|	WAIT { addByte(currBlock, $1); }
|	SHARED { addByte(currBlock, $1); } methodCall
|	SYNC { addByte(currBlock, $1); } methodCall
|	NEWLOCK { addByte(currBlock, $1); }

|	POP { addByte(currBlock, $1); }
|	NOP { addByte(currBlock, $1); } 
|	BRFALSE ID { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }
/*|	BR ID { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }*/
|	MOD { addByte(currBlock, $1); }

|	CEQ { addByte(currBlock, $1); }
|	CGT { addByte(currBlock, $1); }
|	CLT { addByte(currBlock, $1); }
|	AND { addByte(currBlock, $1); }
|	OR { addByte(currBlock, $1); }

|	NOT { addByte(currBlock, $1); }
|	NEWARR type { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }
|	LDLEN { addByte(currBlock, $1); }
|	LDELEM { addByte(currBlock, $1); }
|	STELEM { addByte(currBlock, $1); }

|	NEWOBJ ID { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }
	newobj
|	CALL { addByte(currBlock, $1); } methodCall
|	LDFLD ID { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }
|	STFLD ID { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }
|	PRINTINT { addByte(currBlock, $1); }

|	PRINTCHAR { addByte(currBlock, $1); }
|	PRINTBOOL { addByte(currBlock, $1); }
|	DUP { addByte(currBlock, $1); }
/*|	RET { addByte(currBlock, $1); }*/
;

newobj: /*nothing*/ { addByte(currBlock, addConstant(currClass, NULL)); }
|	LPAR4 type RPAR4 { addByte(currBlock, addConstant(currClass, $2)); }

endBlock: RET { addByte(currBlock, $1); }
|	BR ID { addByte(currBlock, $1); addByte(currBlock, addConstant(currClass, $2)); }
;	

methodCall: type ID { currSig = newSignature($1, $2); } LPAR methodArgs RPAR
	{ addByte(currBlock, addConstant(currClass, currSig)); }
;

methodArgs: /*nothing*/
|	type { addArgType(currSig, $1); }
|	type COMMA { addArgType(currSig, $1); } methodArgs
;

type: TYPE_INT { $$ = newType($1); }
|	TYPE_CHAR { $$ = newType($1); }
|	TYPE_BOOL { $$ = newType($1); }
|	TYPE_THREAD LPAR4 type RPAR4 { $$ = newType($1); setElemType($$, $3); }
|	TYPE_LOCK { $$ = newType($1); }
|	ID { $$ = newType(T_CLASS); setClassName($$, $1); }
|	ID LPAR4 type RPAR4 { $$ = newType(T_CLASS); setClassName($$, $1); setElemType($$, $3); }
|	type LPAR3 RPAR3 { $$ = newType(T_ARRAY); setElemType($$, $1); }
;
	
%%




Class* parse(FILE* file)
{
	yyin = file;
	yyparse();
	
	return currClass;
}

void yyerror(const char* str)
{
	currClass = NULL;
//	fprintf(stdout, "%s\n", str);
}
