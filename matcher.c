
#include "matcher.h"

#include <stdio.h>

ASTNode constant_folding(ASTNode node) {
	if (!node) return NULL;

	ASTNode l = constant_folding(node->left);
	ASTNode r = constant_folding(node->right);

	if (node->kind != AST_OP_ADD && node->kind != AST_OP_MUL) return node;
	if (!is_constant_ast_node(l) || !is_constant_ast_node(r)) return node;

	ASTNode result = NULL;
	if (node->kind == AST_OP_ADD) {
		result = create_ast_node(AST_INTEGER, l->value + r->value);
	} else if (node->kind == AST_OP_MUL) {
		result = create_ast_node(AST_INTEGER, l->value * r->value);
	}

	if (is_left(node)) {
		set_ast_node_left(node->parent, result);
	} else if (is_right(node)) {
		set_ast_node_right(node->parent, result);
	}

	destroy_ast_node(node->left);
	destroy_ast_node(node->right);
	destroy_ast_node(node);

	return result;
}
