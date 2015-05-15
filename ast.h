
#ifndef __AST_H_INCLUDED__
#define __AST_H_INCLUDED__

enum {
	AST_INTEGER,
	AST_OP_ADD,
	AST_OP_SUB,
	AST_OP_MUL,
	AST_OP_DIV,
	AST_OP_EQ,
};

typedef struct __ast_node *ast_node;
struct __ast_node {
	int kind;
	int value;

	ast_node left;
	ast_node right;

	ast_node parent;
};

/* creation and destruction */
ast_node create_ast_node(int kind, int value);
void destroy_ast_node(ast_node n);

void set_ast_node_left(ast_node parent, ast_node child);
void set_ast_node_right(ast_node parent, ast_node child);

/* for debug. */
void show_ast_node(ast_node n);

#endif /* __AST_H_INCLUDED__ */
