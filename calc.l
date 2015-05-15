
%{

#include "ast.h"
#include "calc.tab.h"

int yywrap() {
	return 1;
}

%}

%%

"+" return (ADD);
"-" return (SUB);
"/" return (DIV);
"*" return (MUL);
"(" return (LP);
")" return (RP);
"=" return (EQ);
":=" return (ASSIGN);

[[:digit:]]+ {
	sscanf(yytext, "%d", &yylval.integer);
	return (INTEGER);
}

[[:blank:]]+ { }
"\n" { return (LF); }

%%
