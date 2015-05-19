
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

Rule create_rule(ASTNode pattern, ASTNode goal, ASTNode freeVars);

ASTNode apply_rule(Rule rule, ASTNode node);

ASTNode constant_folding(ASTNode node);

#endif /* __MATCHER_H_INCLUDED__ */
