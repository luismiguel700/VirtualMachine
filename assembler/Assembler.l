%{
#include "y.tab.h"
#include "../metadata/Instructions.h"
#include "../metadata/Type.h"
%}



%%



"class" { return CLASS; }
"field" { return FIELD; }
"method" { return METHOD; }
"locals" { return LOCALS; }
"empty" { return EMPTY; }
"(" { return LPAR; }
")" { return RPAR; }
"{" { return LPAR2; }
"}" { return RPAR2; }
"[" { return LPAR3; }
"]" { return RPAR3; }
"<" { return LPAR4; }
">" { return RPAR4; }
"," { return COMMA; }
":" { return COLON; }
";" { return SEMICOLON; }

"int" { yylval.integer = T_INT; return TYPE_INT; }
"char" { yylval.integer = T_CHAR; return TYPE_CHAR; }
"bool" { yylval.integer = T_BOOL; return TYPE_BOOL; }
"thread" { yylval.integer = T_THREAD; return TYPE_THREAD; }
"lock" { yylval.integer = T_LOCK; return TYPE_LOCK; }

"ldint" { yylval.byte = C_LDINT; return LDINT; }
"ldchar" { yylval.byte = C_LDCHAR; return LDCHAR; }
"ldbool" { yylval.byte = C_LDBOOL; return LDBOOL; }
"add" { yylval.byte = C_ADD; return ADD; }
"sub" { yylval.byte = C_SUB; return SUB; }

"mul" { yylval.byte = C_MUL; return MUL; }
"div" { yylval.byte = C_DIV; return DIV; }
"ldarg" { yylval.byte = C_LDARG; return LDARG; }
"ldloc" { yylval.byte = C_LDLOC; return LDLOC; }
"stloc" { yylval.byte = C_STLOC; return STLOC; }

"fork" { yylval.byte = C_FORK; return FORK; }
"wait" { yylval.byte = C_WAIT; return WAIT; }
"shared" { yylval.byte = C_SHARED; return SHARED; }
"sync" { yylval.byte = C_SYNC; return SYNC; }
"newlock" { yylval.byte = C_NEWLOCK; return NEWLOCK; }

"pop" { yylval.byte = C_POP; return POP; }
"nop" { yylval.byte = C_NOP; return NOP; }
"brfalse" { yylval.byte = C_BRFALSE; return BRFALSE; }
"br" { yylval.byte = C_BR; return BR; }
"mod" { yylval.byte = C_MOD; return MOD; }

"ceq" { yylval.byte = C_CEQ; return CEQ; }
"cgt" { yylval.byte = C_CGT; return CGT; }
"clt" { yylval.byte = C_CLT; return CLT; }
"and" { yylval.byte = C_AND; return AND; }
"or" { yylval.byte = C_OR; return OR; }

"not" { yylval.byte = C_NOT; return NOT; }
"newarr" { yylval.byte = C_NEWARR; return NEWARR; }
"ldlen" { yylval.byte = C_LDLEN; return LDLEN; }
"ldelem" { yylval.byte = C_LDELEM; return LDELEM; }
"stelem" { yylval.byte = C_STELEM; return STELEM; }

"newobj" { yylval.byte = C_NEWOBJ; return NEWOBJ; }
"call" { yylval.byte = C_CALL; return CALL; }
"ldfld" { yylval.byte = C_LDFLD; return LDFLD; }
"stfld" { yylval.byte = C_STFLD; return STFLD; }
"printint" { yylval.byte = C_PRINTINT; return PRINTINT; }

"printchar" { yylval.byte = C_PRINTCHAR; return PRINTCHAR; }
"printbool" { yylval.byte = C_PRINTBOOL; return PRINTBOOL; }
"dup" { yylval.byte = C_DUP; return DUP; }
"ret" { yylval.byte = C_RET; return RET; }

[0-9]+ { yylval.integer = atoi(yytext); return INT; }
\'[A-Za-z0-9 ]\' { yylval.byte = yytext[1]; return CHAR; }
"true" { yylval.integer = 1; return BOOL; }
"false" { yylval.integer = 0; return BOOL; }
[A-Za-z][A-Za-z0-9]* { yylval.string = strdup(yytext); return ID; }
[ \t\n] { ; }



%%



int yywrap(void)
{
	return 1;
}
