
#include "symtable.h"

SymbolTable symbols;

int main(int argc, char *argv[]) {
	symbols = create_sym_table();
	return yyparse();
}