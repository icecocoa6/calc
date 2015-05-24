
#include "matcher.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "compute.h"



/* prototypes for private functions */
int pattern_matching(ASTNode node, ASTNode pattern, Rule rule);
ASTNode bind_free_variables(ASTNode tree, ASTNode bindings[]);
void substitute_variables(ASTNode tree, int from, int to);
int has_contradiction(Rule rule);

int matches(ASTNode node, Rule rule);
ASTNode create_bound_goal(Rule rule);


Rule create_rule(ASTNode pattern, ASTNode goal, ASTNode cond, ASTNode freeVars) {
	Rule r = (Rule)calloc(1, sizeof(struct __rule));
	r->numOfFreeVars = 0;
	r->pattern = copy_ast_node(pattern);
	r->goal = copy_ast_node(goal);
	r->condition = (cond) ? copy_ast_node(cond) : NULL;

	while (freeVars) {
		substitute_variables(r->pattern, freeVars->value, r->numOfFreeVars);
		substitute_variables(r->goal, freeVars->value, r->numOfFreeVars);
		if (r->condition) {
			substitute_variables(r->condition, freeVars->value, r->numOfFreeVars);
		}
		r->numOfFreeVars++;

		freeVars = freeVars->left;
	}

	r->bindings = calloc(r->numOfFreeVars, sizeof(Rule));

	return r;
}

Rule copy_rule(Rule rule) {
	Rule r = (Rule)calloc(1, sizeof(struct __rule));
	r->numOfFreeVars = rule->numOfFreeVars;
	r->pattern = copy_ast_node(rule->pattern);
	r->goal = copy_ast_node(rule->goal);
	r->condition = (rule->condition) ? copy_ast_node(rule->condition) : NULL;
	r->bindings = calloc(r->numOfFreeVars, sizeof(Rule));
	return r;
}

void destroy_rule(Rule rule) {
	destroy_ast_node(rule->pattern);
	destroy_ast_node(rule->goal);
	if (rule->condition) destroy_ast_node(rule->condition);
	free(rule->bindings);
	free(rule);
}

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

void substitute_variables(ASTNode tree, int from, int to) {
	if (!tree) return;
	if (tree->kind == AST_VAR && tree->value == from) {
		tree->kind = AST_FREE_VAR;
		tree->value = to;
	}

	substitute_variables(tree->left, from, to);
	substitute_variables(tree->right, from, to);
}

int has_contradiction(Rule rule) {
	if (!rule->condition) return 0;

	ASTNode node = bind_free_variables(rule->condition, rule->bindings);
	node = compute(node, rules, rules_count, 0);
	int failed = is_constant_ast_node(node) && (node->value == 0);

	destroy_ast_node(node);

	return failed;
}

//////
ASTNode create_bound_goal(Rule rule) {
	return bind_free_variables(rule->goal, rule->bindings);
}

ASTNode apply_rule(Rule rule, ASTNode tree) {
	if (!tree) return NULL;

	if (matches(tree, rule)) {
		if (has_contradiction(rule)) return NULL;
		return create_bound_goal(rule);
	}

	ASTNode t = apply_rule(rule, tree->left);
	if (t) {
		ASTNode s = create_ast_node(tree->kind, tree->value);
		s->parent = tree->parent;
		set_ast_node_left(s, t);
		if (tree->right) set_ast_node_right(s, tree->right);
		return s;
	}

	t = apply_rule(rule, tree->right);
	if (t) {
		ASTNode s = create_ast_node(tree->kind, tree->value);
		s->parent = tree->parent;
		if (tree->left) set_ast_node_left(s, tree->left);
		set_ast_node_right(s, t);
		return s;
	}

	return NULL;
}

ASTNode constant_folding(ASTNode node) {
	if (!node) return NULL;

	ASTNode l = constant_folding(node->left);
	ASTNode r = constant_folding(node->right);

	if (!is_computable_op(node)) return node;
	if (!is_constant_ast_node(l) || !is_constant_ast_node(r)) return node;

	ASTNode result = NULL;
	if (node->kind == AST_OP_ADD) {
		result = create_ast_node(AST_INTEGER, l->value + r->value);
	} else if (node->kind == AST_OP_MUL) {
		result = create_ast_node(AST_INTEGER, l->value * r->value);
	} else if (node->kind == AST_OP_EQ) {
		result = create_ast_node(AST_INTEGER, l->value == r->value);
	} else if (node->kind == AST_OP_LT) {
		result = create_ast_node(AST_INTEGER, l->value < r->value);
	} else if (node->kind == AST_OP_AND) {
		result = create_ast_node(AST_INTEGER, l->value && r->value);
	} else if (node->kind == AST_OP_OR) {
		result = create_ast_node(AST_INTEGER, l->value || r->value);
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
