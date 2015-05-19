
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "matcher.h"
#include "symtable.h"


void check(int br) {
	if (br) printf(".");
	else {
		printf("Failed.\n");
		exit(-1);
	}
}

void testTable1() {
	printf("%s\n", __func__);
	SymbolTable table = create_sym_table();

	check(register_sym_table(table, "x") == 0);
	check(get_sym_index(table, "x") == 0);
	check(get_sym_index(table, "X") == -1);

	check(register_sym_table(table, "x") == 0);
	check(register_sym_table(table, "hoge") == 1);
	check(get_sym_index(table, "x") == 0);
	check(get_sym_index(table, "hoge") == 1);

	destroy_sym_table(table);

	printf("\n");
}

void test1() {
	/* 5 := 5 */
	ASTNode a = create_ast_node(AST_INTEGER, 5);
	ASTNode b = create_ast_node(AST_INTEGER, 5);
	struct __rule rule;
	rule.pattern = a;
	rule.numOfFreeVars = 0;

	printf("%d\n", matches(b, &rule));
}


void test2() {
	/* a - b := a + (-b) */
	ASTNode ptn = create_ast_node(AST_OP_SUB, 0);
	set_ast_node_left(ptn, create_ast_node(AST_FREE_VAR, 0));
	set_ast_node_right(ptn, create_ast_node(AST_FREE_VAR, 1));

	ASTNode goal = create_ast_node(AST_OP_ADD, 0);
	set_ast_node_left(goal, create_ast_node(AST_FREE_VAR, 0));
	{
		ASTNode m = create_ast_node(AST_OP_MUL, 0);
		set_ast_node_left(m, create_ast_node(AST_INTEGER, -1));
		set_ast_node_right(m, create_ast_node(AST_FREE_VAR, 1));
		set_ast_node_right(goal, m);
	}

	ASTNode tree = create_ast_node(AST_OP_SUB, 0);
	set_ast_node_left(tree, create_ast_node(AST_INTEGER, 6));
	set_ast_node_right(tree, create_ast_node(AST_INTEGER, 9));
	struct __rule rule;
	rule.pattern = ptn;
	rule.goal = goal;
	rule.numOfFreeVars = 1;
	rule.bindings = malloc(sizeof(ASTNode));

	printf("%d\n", matches(tree, &rule));

	show_ast_node(tree);
	printf(" :- ");
	show_ast_node(create_bound_goal(&rule));
	printf("\n");
}

void testPattern1() {
	ASTNode ptn = create_ast_node(AST_VAR, 0);
	ASTNode goal = create_ast_node(AST_INTEGER, 10);
	Rule rule = create_rule(ptn, goal, NULL);

	ASTNode tree= create_ast_node(AST_OP_ADD, 0);
	set_ast_node_left(tree, create_ast_node(AST_VAR, 0));
	set_ast_node_right(tree, create_ast_node(AST_INTEGER, 4));
}

SymbolTable symbols;
int rules_count = 0;
Rule rules[256] = { NULL };

int main(int argc, char *argv[]) {
	symbols = create_sym_table();
	printf("test1: 5 := 5\n");
	test1();

	printf("test2: a - b := a + (-b)\n");
	test2();

	testTable1();

	testPattern1();

	return 0;
}