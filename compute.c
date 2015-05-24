
#include "compute.h"

#include <stdio.h>
#include <stdlib.h>

int rules_count = 0;
Rule rules[256] = { NULL };


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
		printf("\t->");
		show_ast_node(t);
		printf("\n");
	}

	for (int i = 0; i < numrules; i++) {
		destroy_rule(rules[i]);
	}
	free(rules);

	return t;
}
