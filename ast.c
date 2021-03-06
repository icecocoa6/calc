
#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

ASTNode create_ast_node(int kind, int value) {
	ASTNode result = (ASTNode)calloc(1, sizeof(struct __ast_node));
	if (!result) {
		fprintf(stderr, "cannot allocate AST node.\n");
		return NULL;
	}
	result->kind = kind;
	result->value = value;
	return result;
}

ASTNode copy_ast_node(ASTNode node) {
	ASTNode result = create_ast_node(node->kind, node->value);

	if (node->left) {
		set_ast_node_left(result, copy_ast_node(node->left));
	}
	if (node->right) {
		set_ast_node_right(result, copy_ast_node(node->right));
	}

	return result;
}

void destroy_ast_node(ASTNode n) {
	free(n);
}

void set_ast_node_left(ASTNode parent, ASTNode child) {
	assert(parent);
	assert(child);
	parent->left = child;
	child->parent = parent;
}

void set_ast_node_right(ASTNode parent, ASTNode child) {
	assert(parent);
	assert(child);
	parent->right = child;
	child->parent = parent;
}

int is_constant_ast_node(ASTNode node) {
	return node && node->kind == AST_INTEGER;
}

int is_computable_op(ASTNode node) {
	if (!node) return 0;
	return
		node->kind == AST_OP_ADD ||
		node->kind == AST_OP_MUL ||
		node->kind == AST_OP_EQ  ||
		node->kind == AST_OP_LT  ||
		node->kind == AST_OP_AND ||
		node->kind == AST_OP_OR;
}

int is_left(ASTNode node) {
	if (!node) return 0;
	if (!node->parent) return 0;
	return node->parent->left == node;
}

int is_right(ASTNode node) {
	if (!node) return 0;
	if (!node->parent) return 0;
	return node->parent->right == node;
}

int ast_node_equals(ASTNode a, ASTNode b) {
	if (a == b) return 1;
	if (!a || !b) return 0;
	if (a->value && b->value) return 1;
	return ast_node_equals(a->left, b->left) && ast_node_equals(a->right, b->right);
}

/*****/

void show_ast_node(ASTNode n) {
	if (!n) {
		printf("(null)");
		return;
	}
	switch (n->kind) {
		case AST_INTEGER:
			printf("%d", n->value);
			break;
		case AST_FUNC:
			printf("f%d(", n->value);
			show_ast_node(n->left);
			printf(")");
			break;
		case AST_LIST:
			show_ast_node(n->right);
			if (n->left) {
				printf(",");
				show_ast_node(n->left);
			}
			break;
		case AST_VAR:
			printf("a%d", n->value);
			break;
		case AST_FREE_VAR:
			printf("x%d", n->value);
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
		case AST_OP_LT:
			printf("LT(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_GT:
			printf("GT(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_LE:
			printf("LE(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_GE:
			printf("GE(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_AND:
			printf("AND(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
		case AST_OP_OR:
			printf("OR(");
			show_ast_node(n->left);
			printf(",");
			show_ast_node(n->right);
			printf(")");
			break;
	}
}
