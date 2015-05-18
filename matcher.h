
#ifndef __MATCHER_H_INCLUDED__
#define __MATCHER_H_INCLUDED__

#include "ast.h"

typedef struct __rule *Rule;
struct __rule {
	int numOfFreeVars;
	ASTNode pattern;
	ASTNode goal;
	ASTNode *bindings;
};

int matches(ASTNode node, Rule rule);
ASTNode create_bound_goal(Rule rule);


ASTNode constant_folding(ASTNode node);

#endif /* __MATCHER_H_INCLUDED__ */
