
%{
	
#include <stdio.h>

#include "ast.h"

%}

%union {
	int integer;
	char *string;
	ast_node node;
}

%defines
%token<integer> INTEGER
%type<node> expr
%token ADD "+"
%token SUB "-"
%token MUL "*"
%token DIV "/"
%token EQ "="
%token LP "("
%token RP ")"
%token ASSIGN ":="
%token LF "\n"
%left "+" "-"
%left "="
%left "*" "/"

%%

input : expr "\n"
      {
          show_ast_node($1);
          printf("\n");
      }

expr : INTEGER
     {
         $$ = create_ast_node(AST_INTEGER, $1);
     }
     | expr "+" expr
     {
         ast_node n = create_ast_node(AST_OP_ADD, 0);
         set_ast_node_left(n, $1);
         set_ast_node_right(n, $3);
         $$ = n;
     }
     | expr "-" expr
     {
         ast_node n = create_ast_node(AST_OP_SUB, 0);
         set_ast_node_left(n, $1);
         set_ast_node_right(n, $3);
         $$ = n;
     }
     | expr "*" expr
     {
         ast_node n = create_ast_node(AST_OP_MUL, 0);
         set_ast_node_left(n, $1);
         set_ast_node_right(n, $3);
         $$ = n;
     }
     | expr "/" expr
     {
         ast_node n = create_ast_node(AST_OP_DIV, 0);
         set_ast_node_left(n, $1);
         set_ast_node_right(n, $3);
         $$ = n;
     }
     | "-" expr
     {
         ast_node n = create_ast_node(AST_OP_MUL, 0);
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
         ast_node n = create_ast_node(AST_OP_EQ, 0);
         set_ast_node_left(n, $1);
         set_ast_node_right(n, $3);
         $$ = n;
     }
;

%%

int yyerror(char *s) {
	fprintf(stderr, "%s\n", s);
	return 0;
}
