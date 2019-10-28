%{
/****************************************************************************
parser.y
ParserWizard generated YACC file.
****************************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include "lexer.h"
#include "tree.h"
using namespace std;
tree parserTree;
%}
%token ASSIGN EQ GT LT
%token GE LE NZ ADD SUB
%token MUL DIV MOD POW INC DEC
%token BITWISEOR BITWISEADD LSHIFT
%token RSHIFT AND OR LNOT LBRACE RBRACE LP
%token RP LSBRACKET RSBRACKET SEMICOLON
%token COMMA POINT SQUITO DQUITO
%token NOTE NUMBER ERROR ID STR
%token VOID IF ELSE FOR SWITCH MAIN CONTINUE BREAK CASE DEFAULT WHILE NEW RETURN
%token INT DOUBLE FLOAT STRING LONG SHORT BYTE 

/////////////////////////////////////////////////////////////////////////////
// declarations section

// parser name
%name parser

// class definition
{
    // place any extra class members here
}

// constructor
{
    // place any extra initialisation code here
}

// destructor
{
    // place any extra cleanup code here
}

// attribute type
%include {
#ifndef YYSTYPE
#define YYSTYPE node*
#endif
}

// place any declarations here

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

// place your YACC rules here (there must be at least one)
    program: declaration_group{parserTree.root = $1;}
    ;
    declaration_group: declaration_block declaration_group {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("declaration_group", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | {$$ = 0;}
    ;
    declaration_block: declaration_var {$$ = $1;}
    | declaration_function {$$ = $1;}
    | main_function {$$ = $1;}
    ;
    main_function: var_type main LP paramester_list RP LBRACE statement RBRACE {$$ = new node("main_function", 0, new(node*[4]){$1, $2, $4, $7}, 4);}
    ;
    main: MAIN {$$ = new node("main", 0, 0, 0);}
    ;
    declaration_var: var_type ID SEMICOLON {$$ = new node("declaration_var", 0, new(node*[2]){$1, $2}, 2);}
    | var_type ID assign NUMBER SEMICOLON {$$ = new node("declaration_var", 0, new(node*[4]){$1, $2, $3, $4}, 4);}
    ;
    declaration_function: var_type ID LP paramester_list RP LBRACE statement RBRACE {$$ = new node("declaration_function", 0, new(node*[4]){$1, $2, $4, $7}, 4);}
    ;
    paramester_list: paramester COMMA paramester_list {int cNodeLength = 0;
    if($3 != 0){cNodeLength = $3->cNodeLength;}
    $$ = new node("paramester_list", 0, parserTree.unit_node($1, $3), cNodeLength + 1);}
    | paramester {$$ = new node("paramester_list", 0, new(node*[1]){$1}, 1);}
    | {$$ = 0;}
    ;
    paramester: var_type ID {$$ = new node("paramester", 0, new(node*[2]){$1, $2}, 2);}
    ;

    statement: declaration_var statement {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | conditional_statement statement {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | loop_statement statement {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | expression SEMICOLON statement {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $3), cNodeLength + 1);}
    | {$$ = 0;}
    ;

    conditional_statement: if_statement {$$ = new node("conditional_statement", 0, new(node*[1]){$1}, 1);}
    | if_statement eles_statement {$$ = new node("conditional_statement", 0, new(node*[2]){$1, $2}, 2);}
    ;
    if_statement: IF LP relational_expression RP LBRACE statement RBRACE {$$ = new node("if_statement", 0, new(node*[2]){$3, $6}, 2);}
    ;
    eles_statement: ELSE LBRACE statement RBRACE {$$ = new node("else_statement", 0, new(node*[1]){$3}, 1);}
    ;

    loop_statement: for_statememt {$$ = $1;}
    | while_statement {$$ = $1;}
    ;
    for_statememt: FOR LP  RP LBRACE statement RBRACE {$$ = new node("for_statement", 0, new(node*[1]){$5}, 1);}
    ;
    while_statement: WHILE LP relational_expression RP LBRACE statement RBRACE {$$ = new node("while_statement", 0, new(node*[2]){$3, $6}, 2);}
    ;

    var_type: INT {$$ = new node("int", 0, 0, 0);}
    ;
    assign: ASSIGN {$$ = new node("=", 0, 0, 0);}
    ;
    
    expression: assignment_expression {$$ = $1;}
    | logical_or_expression {$$ = $1;}
    | logical_and_expression {$$ = $1;}
    | inclusive_or_expression {$$ = $1;}
    | and_expression {$$ = $1;}
    | equality_expression {$$ = $1;}
    | relational_expression {$$ = $1;}
    | shift_expression {$$ = $1;}
    | additive_expression {$$ = $1;}
    | multiplicative_expression {$$ = $1;}
    | power_expression {$$ = $1;}
    | cast_expression {$$ = $1;}
    | unary_expression {$$ = $1;}
    | assignment_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | logical_or_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | logical_and_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | inclusive_or_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | and_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | equality_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | shift_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | additive_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | multiplicative_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | power_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | cast_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    | unary_expression COMMA expression {$$ = new node("expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
	

    assignment_expression: ID ASSIGN expression {$$ = new node("assignment_expression", 0, new(node*[2]){$1, $3}, 2);}      
    ;
    
	    
    logical_or_expression: logical_and_expression {$$ = $1;}
    | logical_or_expression OR logical_and_expression {$$ = new node("logical_or_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    logical_and_expression: inclusive_or_expression {$$ = $1;}
    | logical_and_expression AND inclusive_or_expression {$$ = new node("logical_and_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;

    
    inclusive_or_expression: and_expression {$$ = $1;}
    | inclusive_or_expression BITWISEOR and_expression {$$ = new node("inclusive_or_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    //exclusive_or_expression
    //: and_expression {$$ = $1;}
    //| exclusive_or_expression '^' and_expression {$$ = new node("exclusive_or_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
    //;
    
    and_expression: equality_expression {$$ = $1;}
    | and_expression BITWISEADD equality_expression {$$ = new node("and_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    equality_expression: relational_expression {$$ = $1;}
    | equality_expression EQ relational_expression {$$ = new node("equality_expression", 0, new(node*[2]){$1, $3}, 2);}
    | equality_expression NZ relational_expression {$$ = new node("not_equality_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    relational_expression: shift_expression {$$ = $1;}
    | relational_expression LT shift_expression {$$ = new node("lt_relational_expression", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression GT shift_expression {$$ = new node("gt_relational_expression", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression LE shift_expression {$$ = new node("le_relational_expression", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression GE shift_expression {$$ = new node("ge_relational_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
	    
    shift_expression: additive_expression {$$ = $1;}
    | shift_expression LSHIFT additive_expression {$$ = new node("lshift_expression", 0, new(node*[2]){$1, $3}, 2);}
    | shift_expression RSHIFT additive_expression {$$ = new node("rshift_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    additive_expression: multiplicative_expression {$$ = $1;}
    | additive_expression ADD multiplicative_expression {$$ = new node("add_expression", 0, new(node*[2]){$1, $3}, 2);}
    | additive_expression SUB multiplicative_expression {$$ = new node("sub_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    multiplicative_expression: power_expression {$$ = $1;}
    | multiplicative_expression MUL power_expression {$$ = new node("mul_expression", 0, new(node*[2]){$1, $3}, 2);}
    | multiplicative_expression DIV power_expression {$$ = new node("div_expression", 0, new(node*[2]){$1, $3}, 2);}
    | multiplicative_expression MOD power_expression {$$ = new node("mod_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    power_expression: cast_expression {$$ = $1;}
    | power_expression POW cast_expression {$$ = new node("power_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    cast_expression: unary_expression {$$ = $1;}
    | var_type LP cast_expression RP {$$ = new node("cast_expression", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    unary_expression: LP expression RP {$$ = $2;}
    | inc ID {$$ = new node("linc_expression", 0, new(node*[2]){$1,$2}, 2);}
    | dec ID {$$ = new node("ldec_expression", 0, new(node*[2]){$1,$2}, 2);}
    | ID inc {$$ = new node("rinc_expression", 0, new(node*[2]){$1,$2}, 2);}
    | ID dec {$$ = new node("rdec_expression", 0, new(node*[2]){$1,$2}, 2);}
    | NUMBER {$$ = $1;}
    | ID {$$ = $1;}
    | not cast_expression {$$ = new node("unary_expression", 0, new(node*[2]){$1,$2}, 2);}
    //| SIZEOF unary_expression
    //| SIZEOF '(' type_name ')'
    ;
    not: LNOT {$$ = new node(" ! ", 0, 0, 0);}
    ;
    inc: INC {$$ = new node(" ++ ", 0, 0, 0);}
    ;
    dec: DEC {$$ = new node(" -- ", 0, 0, 0);}
    ;
 	    

%%

/////////////////////////////////////////////////////////////////////////////
// programs section
int main(void)
{
    lexer lexer;
    string sFile;
    cin >> sFile;
    lexer.yyin = new std::ifstream(sFile);
    parser parser;
    int n = 1;
    if (parser.yycreate(&lexer)) {
        if (lexer.yycreate(&parser)) {
            n = parser.yyparse();
        }
    }
    parserTree.print(parserTree.root, "");
    return n;
}

