
#ifndef __SYMTABLE_H_INCLUDED__
#define __SYMTABLE_H_INCLUDED__

struct __sym_table {
	int numSymbol;
	char *symbols[1024];
};
typedef struct __sym_table *SymbolTable;


SymbolTable create_sym_table();
void destroy_sym_table(SymbolTable table);

int register_sym_table(SymbolTable table, char *symbol);
int get_sym_index(SymbolTable table, char *symbol);


#endif /* __SYMTABLE_H_INCLUDED__ */
