
#include "matcher.h"

#include <stdio.h>
#include <string.h>

/* prototypes for private functions */
int pattern_matching(ASTNode node, ASTNode pattern, Rule rule);
ASTNode bind_free_variables(ASTNode tree, ASTNode bindings[]);


int matches(ASTNode node, Rule rule) {
	memset(rule->bindings, 0, sizeof(ASTNode) * rule->numOfFreeVars);
	return pattern_matching(node, rule->pattern, rule);
}

int pattern_matching(ASTNode node, ASTNode pattern, Rule rule) {
	if ((!node && pattern) || (node && !pattern)) return 0;
	if (!node && !pattern) return 1;

	if (pattern->kind == AST_FREE_VAR) {
		if (rule->bindings[pattern->value]) {
			return pattern_matching(node, rule->bindings[pattern->value], rule);
		} else {
			rule->bindings[pattern->value] = node;
			return 1;
		}
	}

	if (node->kind != pattern->kind || node->value != pattern->value) return 0;

	return pattern_matching(node->left, pattern->left, rule) &&
			pattern_matching(node->right, pattern->right, rule);
}

ASTNode bind_free_variables(ASTNode tree, ASTNode bindings[]) {
	if (tree->kind == AST_FREE_VAR) {
		return copy_ast_node(bindings[tree->value]);
	}

	ASTNode result = create_ast_node(tree->kind, tree->value);
	if (tree->left) {
		ASTNode left = bind_free_variables(tree->left, bindings);
		set_ast_node_left(result, left);
	}
	if (tree->right) {
		ASTNode right = bind_free_variables(tree->right, bindings);
		set_ast_node_right(result, right);
	}

	return result;
}

ASTNode create_bound_goal(Rule rule) {
	return bind_free_variables(rule->goal, rule->bindings);
}


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
