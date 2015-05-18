
%{
	
#include <stdio.h>

#include "ast.h"

extern ASTNode constant_folding(ASTNode node);

%}

%union {
	int integer;
	char string[64];
	ASTNode node;
}

%defines
%token<integer> INTEGER
%token<string> SYMBOL
%token ADD "+"
%token SUB "-"
%token MUL "*"
%token DIV "/"
%token EQ "="
%token LP "("
%token RP ")"
%token ASSIGN ":="
%token COMMA ","
%token LF "\n"

%type<node> expr
%type<node> expr_list

%left "+" "-"
%left "="
%left "*" "/"

%%

input
    : line
    | input line
    ;

line
    : expr "\n"
    {
        show_ast_node($1);
        printf("\n\t-> ");
        show_ast_node(constant_folding($1));
        printf("\n");
    }

expr
    : INTEGER
    {
        $$ = create_ast_node(AST_INTEGER, $1);
    }
    | expr "+" expr
    {
        ASTNode n = create_ast_node(AST_OP_ADD, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "-" expr
    {
        ASTNode n = create_ast_node(AST_OP_SUB, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "*" expr
    {
        ASTNode n = create_ast_node(AST_OP_MUL, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "/" expr
    {
        ASTNode n = create_ast_node(AST_OP_DIV, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | "-" expr
    {
        ASTNode n = create_ast_node(AST_OP_MUL, 0);
        set_ast_node_left(n, create_ast_node(AST_INTEGER, -1));
        set_ast_node_right(n, $2);
        $$ = n;
    }
    | "(" expr ")"
    {
        $$ = $2;
    }
    | "(" expr_list ")"
    {
        $$ = $2;
    }
    | expr "=" expr
    {
        ASTNode n = create_ast_node(AST_OP_EQ, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | SYMBOL expr
    {
        ASTNode n = create_ast_node(AST_FUNC, 0 /* TODO: proper value */);
        set_ast_node_left(n, $2);
        $$ = n;
    }
;

expr_list
    : expr
    {
        $$ = $1;
    }
    | expr_list "," expr
    {
        set_ast_node_left($1, $3);
        $$ = $1;
    }
    ;

%%

int yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	return 0;
}
