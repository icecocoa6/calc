
#include "compute.h"
#include "symtable.h"

#include <stdio.h>
#include <stdlib.h>

int rules_count = 0;
Rule rules[256] = { NULL };

extern SymbolTable symbols;

int priority(ASTNode node) {
	switch(node->kind) {
		case AST_OP_ADD: return 10;
		case AST_OP_SUB: return 10;
		case AST_OP_MUL: return 15;
		case AST_OP_DIV: return 15;
		case AST_OP_EQ: return 5;
		case AST_OP_GT: return 5;
		case AST_OP_GE: return 5;
		case AST_OP_LT: return 5;
		case AST_OP_LE: return 5;
		case AST_OP_AND: return 1;
		case AST_OP_OR: return 1;
	}

	return 100;
}

void printExpression(ASTNode node, int p) {
	if (!node) return;

	if (p > priority(node)) printf("(");
	switch(node->kind) {
		case AST_INTEGER:
			printf("%d", node->value);
			break;
		case AST_FUNC:
			printf("%s(", symbols->symbols[node->value]);
			printExpression(node->left, priority(node));
			printf(")");
			break;
		case AST_VAR:
			printf("%s", symbols->symbols[node->value]);
			break;
		case AST_FREE_VAR:
			printf("x%d\n", node->value);
			break;
		case AST_LIST:
			printExpression(node->right, priority(node));
			if (node->left) {
				printf(", ");
				printExpression(node->left, priority(node));
			}
			break;
		case AST_OP_ADD:
			printExpression(node->left, priority(node));
			printf(" + ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_SUB:
			printExpression(node->left, priority(node));
			printf(" - ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_MUL:
			printExpression(node->left, priority(node));
			printf(" * ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_DIV:
			printExpression(node->left, priority(node));
			printf(" / ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_EQ:
			printExpression(node->left, priority(node));
			printf(" = ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_GT:
			printExpression(node->left, priority(node));
			printf(" > ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_GE:
			printExpression(node->left, priority(node));
			printf(" >= ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_LT:
			printExpression(node->left, priority(node));
			printf(" < ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_LE:
			printExpression(node->left, priority(node));
			printf(" <= ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_AND:
			printExpression(node->left, priority(node));
			printf(" and ");
			printExpression(node->right, priority(node));
			break;
		case AST_OP_OR:
			printExpression(node->left, priority(node));
			printf(" or ");
			printExpression(node->right, priority(node));
			break;
	}
	if (p >= priority(node)) printf(")");
}

ASTNode compute(ASTNode node, Rule *rules_, int numrules, int log) {
	Rule *rules = malloc(sizeof(Rule) * numrules);
	for (int i = 0; i < numrules; i++) {
		rules[i] = copy_rule(rules_[i]);
	}

	if (log) {
		show_ast_node(node);
		printf("\n");
	}

	ASTNode t = node;
	t = constant_folding(t);

	int matched = 1;
	while (matched) {
		matched = 0;

		for (int i = 0; i < numrules; i++) {
			ASTNode tree = apply_rule(rules[i], t);
			if (tree) {
				t = tree;
				matched = 1;
				break;
			}
		}

		if (!matched) break;

		if (log) {
			printf("\t-> ");
			show_ast_node(t);
			printf("\n");
		}

		t = constant_folding(t);

		if (log) {
			printf("\t-> ");
			show_ast_node(t);
			printf("\n");
		}
	}

	if (log) {
		printf("\t-> ");
		printExpression(t, 0);
		printf("\n");
	}

	for (int i = 0; i < numrules; i++) {
		destroy_rule(rules[i]);
	}
	free(rules);

	return t;
}
