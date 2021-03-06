
%{
	
#include <stdio.h>

#include "ast.h"
#include "matcher.h"
#include "symtable.h"
#include "compute.h"

extern ASTNode constant_folding(ASTNode node);
extern SymbolTable symbols;

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
%token HAT "^"
%token AND "and"
%token OR "or"
%token IF "if"
%token LT "<"
%token GT ">"
%token LE "<="
%token GE ">="
%token ASSIGN ":="
%token COMMA ","
%token LF "\n"

%type<node> expr
%type<node> expr_list
%type<node> symbol_list
%type<node> free_vars
%type<node> condition_opt

%left "and" "or"
%left "=" "<" ">" "<=" ">="
%left "+" "-"
%left "*" "/"
%left NEGATE

%%

input
    : line
    | input line
    ;

line
    : expr "\n"
    {
        if ($1) {
            compute($1, rules, rules_count, 1);
        }
    }
    | error "\n"
    {
        yyerrok;
    }
    | "\n"
    {
    
    }
    ;

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
    %prec NEGATE
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
    | expr "=" expr
    {
        ASTNode n = create_ast_node(AST_OP_EQ, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "<" expr
    {
        ASTNode n = create_ast_node(AST_OP_LT, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr ">" expr
    {
        ASTNode n = create_ast_node(AST_OP_GT, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "<=" expr
    {
        ASTNode n = create_ast_node(AST_OP_LE, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr ">=" expr
    {
        ASTNode n = create_ast_node(AST_OP_GE, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "and" expr
    {
        ASTNode n = create_ast_node(AST_OP_AND, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | expr "or" expr
    {
        ASTNode n = create_ast_node(AST_OP_OR, 0);
        set_ast_node_left(n, $1);
        set_ast_node_right(n, $3);
        $$ = n;
    }
    | SYMBOL "(" expr_list ")"
    {
        int idx = register_sym_table(symbols, $1);
        ASTNode n = create_ast_node(AST_FUNC, idx);
        set_ast_node_left(n, $3);
        $$ = n;
    }
    | SYMBOL
    {
        int idx = register_sym_table(symbols, $1);
        $$ = create_ast_node(AST_VAR, idx);
    }
    | free_vars expr ":=" expr condition_opt
    {
        show_ast_node($2);
        printf(" := ");
        show_ast_node($4);
        printf("\n");

        Rule rule = create_rule($2, $4, $5, $1);
        rules[rules_count++] = rule;

        printf("\t->");
        show_ast_node(rule->pattern);
        printf(" := ");
        show_ast_node(rule->goal);
        printf("\n");

        if ($5) {
            printf("\tif ");
            show_ast_node(rule->condition);
            printf("\n");
        }
        $$ = NULL;
    }
;

condition_opt
    :
    {
        $$ = NULL;
    }
    | "if" expr
    {
        $$ = $2;
    }

free_vars
    : "^"
    {
        $$ = NULL;
    }
    | "^" "(" symbol_list ")"
    {
        $$ = $3;
    }

symbol_list
    : SYMBOL
    {
        int idx = register_sym_table(symbols, $1);
        $$ = create_ast_node(AST_VAR, idx);
    }
    | symbol_list "," SYMBOL
    {
        int idx = register_sym_table(symbols, $3);
        $$ = create_ast_node(AST_VAR, idx);
        set_ast_node_left($$, $1);
    }
    ;

expr_list
    : expr
    {
        $$ = create_ast_node(AST_LIST, 0);
        set_ast_node_right($$, $1);
    }
    | expr_list "," expr
    {
        ASTNode n = create_ast_node(AST_LIST, 0);
        set_ast_node_right(n, $3);
        set_ast_node_left($1, n);
        $$ = $1;
    }
    ;

%%

int yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	return 0;
}
