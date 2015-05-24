#ifndef __COMPUTE_H_INCLUDED__
#define __COMPUTE_H_INCLUDED__

#include "ast.h"
#include "matcher.h"

ASTNode compute(ASTNode node, Rule *rules, int numrules, int log);

extern Rule rules[256];
extern int rules_count;

#endif /* __COMPUTE_H_INCLUDED__ */
