
#include "symtable.h"
#include "matcher.h"
#include <stdio.h>

SymbolTable symbols;

int rules_count = 0;
Rule rules[256] = { NULL };

int main(int argc, char *argv[]) {
	symbols = create_sym_table();
	return yyparse();
}