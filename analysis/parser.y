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
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN
%token LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN OR_ASSIGN XOR_ASSIGN
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
    $$ = new node("program", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | {$$ = 0;}
    ;
    declaration_block: main_function {$$ = $1;}
    | declaration_var SEMICOLON {$$ = $1;}
    | declaration_function {$$ = $1;}
    ;
    
    main_function: var_type MAIN LP paramester_list RP LBRACE statement RBRACE {
    $$ = new node("main_function", 0, new(node*[3]){$1, $4, $7}, 3);}
    ;
    declaration_var: var_type ID {$$ = new node("declaration_var", 0, new(node*[2]){$1, $2}, 2);}
    | var_type assignment_expression {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("declaration_var", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | declaration_var COMMA ID {int cNodeLength = 0;
    if($1 != 0){cNodeLength = $1->cNodeLength;}
    $$ = new node("declaration_var", 0, parserTree.unit_node($3, $1), cNodeLength + 1);}
    | declaration_var COMMA assignment_expression {int cNodeLength = 0;
    if($1 != 0){cNodeLength = $1->cNodeLength;}
    $$ = new node("declaration_var", 0, parserTree.unit_node($3, $1), cNodeLength + 1);}
    ;
    declaration_function: var_type ID LP paramester_list RP LBRACE statement RBRACE {
    $$ = new node("declaration_function", 0, new(node*[4]){$1, $2, $4, $7}, 4);}
    ;

    paramester_list: paramester COMMA paramester_list {int cNodeLength = 0;
    if($3 != 0){cNodeLength = $3->cNodeLength;}
    $$ = new node("paramester_list", 0, parserTree.unit_node($1, $3), cNodeLength + 1);}
    | paramester {$$ = new node("paramester_list", 0, new(node*[1]){$1}, 1);}
    | {$$ = 0;}
    ;
    paramester: var_type ID {$$ = new node("paramester", 0, new(node*[2]){$1, $2}, 2);}
    ;

    statement: declaration_var SEMICOLON statement {int cNodeLength = 0;
    if($3 != 0){cNodeLength = $3->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $3), cNodeLength + 1);}
    | conditional_statement statement {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | loop_statement statement {int cNodeLength = 0;
    if($2 != 0){cNodeLength = $2->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $2), cNodeLength + 1);}
    | expression SEMICOLON statement {int cNodeLength = 0;
    if($3 != 0){cNodeLength = $3->cNodeLength;}
    $$ = new node("statement", 0, parserTree.unit_node($1, $3), cNodeLength + 1);}
    | {$$ = 0;}
    ;

    conditional_statement: if_statement {$$ = new node("conditional_statement", 0, new(node*[1]){$1}, 1);}
    | if_statement eles_statement {$$ = new node("conditional_statement", 0, new(node*[2]){$1, $2}, 2);}
    ;
    if_statement: IF LP logical_or_expression RP LBRACE statement RBRACE {
    $$ = new node("if_statement", 0, new(node*[2]){$3, $6}, 2);}
    ;
    eles_statement: ELSE LBRACE statement RBRACE {$$ = new node("else_statement", 0, new(node*[1]){$3}, 1);}
    ;

    loop_statement: for_statememt {$$ = $1;}
    | while_statement {$$ = $1;}
    ;
    for_statememt: FOR LP declaration_var SEMICOLON logical_or_expression SEMICOLON expression RP LBRACE statement RBRACE {
    $$ = new node("for_statement", 0, new(node*[4]){$3, $5, $7, $10}, 4);}
    | FOR LP assignment_expression SEMICOLON logical_or_expression SEMICOLON expression RP LBRACE statement RBRACE {
    $$ = new node("for_statement", 0, new(node*[4]){$3, $5, $7, $10}, 4);}
    ;
    while_statement: WHILE LP logical_or_expression RP LBRACE statement RBRACE {
    $$ = new node("while_statement", 0, new(node*[2]){$3, $6}, 2);}
    ;

    var_type: INT {$$ = new node("int", 0, 0, 0);}
    | DOUBLE {$$ = new node("double", 0, 0, 0);}
    | FLOAT {$$ = new node("float", 0, 0, 0);}
    | STRING {$$ = new node("string", 0, 0, 0);}
    | LONG {$$ = new node("long", 0, 0, 0);}
    | SHORT {$$ = new node("short", 0, 0, 0);}
    | BYTE {$$ = new node("byte", 0, 0, 0);}
    | VOID {$$ = new node("void", 0, 0, 0);}
    ;
    
    expression: assignment_expression {$$ = $1;}
    | logical_or_expression {$$ = $1;}
    | logical_or_expression COMMA expression {$$ = new node(",", 0, new(node*[2]){$1, $3}, 2);}
    | assignment_expression COMMA expression {$$ = new node(",", 0, new(node*[2]){$1, $3}, 2);}
    ;

    assignment_expression: ID ASSIGN expression {$$ = new node("=", 0, new(node*[2]){$1, $3}, 2);}
    | ID MUL_ASSIGN expression {$$ = new node("*=", 0, new(node*[2]){$1, $3}, 2);}
    | ID DIV_ASSIGN expression {$$ = new node("/=", 0, new(node*[2]){$1, $3}, 2);}
    | ID MOD_ASSIGN expression {$$ = new node("%=", 0, new(node*[2]){$1, $3}, 2);}
    | ID ADD_ASSIGN expression {$$ = new node("+=", 0, new(node*[2]){$1, $3}, 2);}
    | ID SUB_ASSIGN expression {$$ = new node("-=", 0, new(node*[2]){$1, $3}, 2);}
    | ID LSHIFT_ASSIGN expression {$$ = new node(">>=", 0, new(node*[2]){$1, $3}, 2);}
    | ID RSHIFT_ASSIGN expression {$$ = new node("<<=", 0, new(node*[2]){$1, $3}, 2);}
    | ID AND_ASSIGN expression {$$ = new node("&=", 0, new(node*[2]){$1, $3}, 2);}
    | ID OR_ASSIGN expression {$$ = new node("|=", 0, new(node*[2]){$1, $3}, 2);}
    | ID XOR_ASSIGN expression {$$ = new node("^=", 0, new(node*[2]){$1, $3}, 2);}
          
    ;
    
    logical_or_expression: logical_and_expression {$$ = $1;}
    | logical_or_expression OR logical_and_expression {$$ = new node("||", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    logical_and_expression: inclusive_or_expression {$$ = $1;}
    | logical_and_expression AND inclusive_or_expression {$$ = new node("&&", 0, new(node*[2]){$1, $3}, 2);}
    ;

    
    inclusive_or_expression: inclusive_and_expression {$$ = $1;}
    | inclusive_or_expression BITWISEOR inclusive_and_expression {$$ = new node("|", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    inclusive_and_expression: equality_expression {$$ = $1;}
    | inclusive_and_expression BITWISEADD equality_expression {$$ = new node("&", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    equality_expression: relational_expression {$$ = $1;}
    | equality_expression EQ relational_expression {$$ = new node("==", 0, new(node*[2]){$1, $3}, 2);}
    | equality_expression NZ relational_expression {$$ = new node("!=", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    relational_expression: shift_expression {$$ = $1;}
    | relational_expression LT shift_expression {$$ = new node("<", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression GT shift_expression {$$ = new node(">", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression LE shift_expression {$$ = new node("<=", 0, new(node*[2]){$1, $3}, 2);}
    | relational_expression GE shift_expression {$$ = new node(">=", 0, new(node*[2]){$1, $3}, 2);}
    ;
        
    shift_expression: additive_expression {$$ = $1;}
    | shift_expression LSHIFT additive_expression {$$ = new node(">>", 0, new(node*[2]){$1, $3}, 2);}
    | shift_expression RSHIFT additive_expression {$$ = new node("<<", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    additive_expression: multiplicative_expression {$$ = $1;}
    | additive_expression ADD multiplicative_expression {$$ = new node("+", 0, new(node*[2]){$1, $3}, 2);}
    | additive_expression SUB multiplicative_expression {$$ = new node("-", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    multiplicative_expression: power_expression {$$ = $1;}
    | multiplicative_expression MUL power_expression {$$ = new node("*", 0, new(node*[2]){$1, $3}, 2);}
    | multiplicative_expression DIV power_expression {$$ = new node("/", 0, new(node*[2]){$1, $3}, 2);}
    | multiplicative_expression MOD power_expression {$$ = new node("%", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    power_expression: cast_expression {$$ = $1;}
    | power_expression POW cast_expression {$$ = new node("^", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    cast_expression: unary_expression {$$ = $1;}
    | var_type LP cast_expression RP {$$ = new node("(type)", 0, new(node*[2]){$1, $3}, 2);}
    ;
    
    unary_expression: declaration_expression {$$ = $1;}
    | INC ID {$$ = new node("l_++", 0, new(node*[1]){$2}, 1);}
    | DEC ID {$$ = new node("l_--", 0, new(node*[1]){$2}, 1);}
    | ID INC {$$ = new node("r_++", 0, new(node*[1]){$1}, 1);}
    | ID DEC {$$ = new node("r_--", 0, new(node*[1]){$1}, 1);}
    | LNOT ID {$$ = new node("!", 0, new(node*[1]){$2}, 1);}
    | LNOT parenthesized_expression {$$ = new node("!", 0, new(node*[1]){$2}, 1);}
    ;

    declaration_expression: parenthesized_expression {$$ = $2;}
    | NUMBER {$$ = $1;}
    | ID {$$ = $1;}
    | STR {$$ = $1;}
    ;

    parenthesized_expression: LP expression RP {$$ = $2;}
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

