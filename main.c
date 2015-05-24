
#include "symtable.h"
#include "matcher.h"
#include <stdio.h>

SymbolTable symbols;

int main(int argc, char *argv[]) {
	symbols = create_sym_table();
	return yyparse();
}