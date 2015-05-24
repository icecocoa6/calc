
#ifndef __AST_H_INCLUDED__
#define __AST_H_INCLUDED__

enum {
	AST_INTEGER,
	AST_FUNC,
	AST_VAR,
	AST_FREE_VAR,
	AST_LIST,
	AST_OP_ADD,
	AST_OP_SUB,
	AST_OP_MUL,
	AST_OP_DIV,
	AST_OP_EQ,
	AST_OP_GT,
	AST_OP_LT,
	AST_OP_GE,
	AST_OP_LE,
	AST_OP_AND,
	AST_OP_OR,
};

typedef struct __ast_node *ASTNode;
struct __ast_node {
	int kind;
	int value;

	ASTNode left;
	ASTNode right;

	ASTNode parent;
};

/* creation and destruction */
ASTNode create_ast_node(int kind, int value);
ASTNode copy_ast_node(ASTNode node);
void destroy_ast_node(ASTNode n);

void set_ast_node_left(ASTNode parent, ASTNode child);
void set_ast_node_right(ASTNode parent, ASTNode child);

/* cheking information */
int is_constant_ast_node(ASTNode node);
int is_computable_op(ASTNode node);

int is_left(ASTNode node);
int is_right(ASTNode node);

int ast_node_equals(ASTNode a, ASTNode b);

/* for debug. */
void show_ast_node(ASTNode n);

#endif /* __AST_H_INCLUDED__ */
