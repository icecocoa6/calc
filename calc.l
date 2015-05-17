
%{

#include <string.h>

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
"," return (COMMA);

[[:digit:]]+ {
	sscanf(yytext, "%d", &yylval.integer);
	return (INTEGER);
}

[[:alpha:]_][[:alnum:]]* {
	strncpy(yylval.string, yytext, 63);
	return (SYMBOL);
}

[[:blank:]]+ { }
"\n" { return (LF); }

%%
