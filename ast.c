
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

ast_node create_ast_node(int kind, int value) {
	ast_node result = (ast_node)calloc(1, sizeof(struct __ast_node));
	if (!result) {
		fprintf(stderr, "cannot allocate AST node.\n");
		return NULL;
	}
	result->kind = kind;
	result->value = value;
	return result;
}

void destroy_ast_node(ast_node n) {
	free(n);
}

void set_ast_node_left(ast_node parent, ast_node child) {
	parent->left = child;
	child->parent = parent;
}

void set_ast_node_right(ast_node parent, ast_node child) {
	parent->right = child;
	child->parent = parent;
}


void show_ast_node(ast_node n) {
	switch (n->kind) {
		case AST_INTEGER:
			printf("%d", n->value);
			break;
		case AST_OP_ADD:
			printf("ADD(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_SUB:
			printf("SUB(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_MUL:
			printf("MUL(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_DIV:
			printf("DIV(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_EQ:
			printf("EQ(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
	}
}
