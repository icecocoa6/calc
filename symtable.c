
#include "symtable.h"

#include <stdlib.h>
#include <string.h>

SymbolTable create_sym_table() {
	SymbolTable table = calloc(1, sizeof(struct __sym_table));
	return table;
}

void destroy_sym_table(SymbolTable table) {
	for (int i = 0; i < table->numSymbol; i++) {
		free(table->symbols[i]);
	}
	free(table);
}

int register_sym_table(SymbolTable table, char *symbol) {
	int idx = get_sym_index(table, symbol);
	if (idx != -1) return idx;

	idx = table->numSymbol;
	table->symbols[idx] = malloc(strlen(symbol) + 1);
	strcpy(table->symbols[idx], symbol);
	table->numSymbol++;
	return idx;
}

int get_sym_index(SymbolTable table, char *symbol) {
	for (int i = 0; i < table->numSymbol; i++) {
		if (strcmp(symbol, table->symbols[i])) continue;
		return i;
	}
	return -1;
}

