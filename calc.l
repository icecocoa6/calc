
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
"^" return (HAT);
">" return (GT);
"<" return (LT);
"<=" return (LE);
">=" return (GE);
":=" return (ASSIGN);
"," return (COMMA);

"and" return (AND);
"or" return (OR);
"if" return (IF);

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
