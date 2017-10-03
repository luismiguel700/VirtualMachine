#ifndef _H_INSTRUCTIONS
#define _H_INSTRUCTIONS

#define C_LDINT 0
#define C_LDCHAR 1
#define C_LDBOOL 2
#define C_ADD 3
#define C_SUB 4

#define C_MUL 5
#define C_DIV 6
#define C_LDARG 7
#define C_LDLOC 8
#define C_STLOC 9

#define C_FORK 10
#define C_WAIT 11
#define C_SHARED 12
#define C_SYNC 13
#define C_NEWLOCK 14

#define C_POP 15
#define C_NOP 16
#define C_BRFALSE 17
#define C_BR 18
#define C_MOD 19

#define C_CEQ 20
#define C_CGT 21
#define C_CLT 22
#define C_AND 23
#define C_OR 24

#define C_NOT 25
#define C_NEWARR 26
#define C_LDLEN 27
#define C_LDELEM 28
#define C_STELEM 29

#define C_NEWOBJ 30
#define C_CALL 31
#define C_LDFLD 32
#define C_STFLD 33
#define C_PRINTINT 34

#define C_PRINTCHAR 35
#define C_PRINTBOOL 36
#define C_DUP 37
#define C_RET 38

#endif
