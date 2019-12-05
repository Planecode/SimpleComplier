%{
/****************************************************************************
parser.y
ParserWizard generated YACC file.
****************************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include "lexer.h"
#include "tree.h"
#include "list.h"
using namespace std;
tree parserTree;
List parserList;
extern map<string, IdValue *> idMap;
%}
%token ASSIGN EQ GT LT
%token MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN SUB_ASSIGN
%token LSHIFT_ASSIGN RSHIFT_ASSIGN AND_ASSIGN OR_ASSIGN XOR_ASSIGN
%token GE LE NZ ADD SUB
%token MUL DIV MOD POW INC DEC
%token BITWISEOR BITWISEADD LSHIFT
%token RSHIFT AND OR LNOT LBRACE RBRACE LP
%token RP LSBRACKET RSBRACKET SEMICOLON
%token COMMA POINT COLON
%token NUMBER ERROR ID STR
%token VOID IF ELSE FOR SWITCH MAIN CONTINUE BREAK CASE DEFAULT WHILE DO NEW RETURN
%token INT DOUBLE FLOAT STRING LONG SHORT BYTE STRUCT

/////////////////////////////////////////////////////////////////////////////
// declarations section

// parser name
%name parser

// class definition
{
    virtual void yyerror(const char YYFAR* text);
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
    program: declaration_group {parserTree.root = $1;}
    ;
    declaration_group: declaration_block declaration_group {
        int cNodeLength = 0;
        if($2 != 0){cNodeLength = $2->cNodeLength;}
        $$ = new node("program", parserTree.unit_node($1, $2), cNodeLength + 1);}
    | {$$ = 0;}
    ;
    declaration_block: main_function {$$ = $1;}
    | init_var SEMICOLON {$$ = $1;}
    | declaration_struct SEMICOLON {$$ = $1;}
    | declaration_function {$$ = $1;}
    ;
    
    main_function: var_type MAIN LP paramester_list RP LBRACE statement RBRACE {
        $$ = new node("main_function", new(node*[3]){$1, $4, $7}, 3);}
    ;

    init_var: declaration_var {$$ = $1;}
    | define_var {$$ = $1;}
    | init_var COMMA ID {
        int cNodeLength = 0;
        if($1 != 0){cNodeLength = $1->cNodeLength;}
        $$ = new node("init_var", parserTree.unit_node($1, $3, 1), cNodeLength + 1);}
    | init_var COMMA assignment_expression {
        int cNodeLength = 0;
        if($1 != 0){cNodeLength = $1->cNodeLength;}
        $$ = new node("init_var", parserTree.unit_node($1, $3, 1), cNodeLength + 1);}
    ;
    declaration_var: var_type var {
        $$ = new node("init_var", new(node*[2]){$1, $2}, 2);
        if($2->description == "id"){idMap[$2->value]->allocate($1->description);}
        else if ($2->description == "array_id"){idMap[$2->cNode[0]->value]->allocate($1->description);}
        }
    ;
    define_var:  var_type assignment_expression {
        $$ = new node("init_var", new(node*[2]){$1, $2}, 2);
        if($2->cNode[0]->description == "id"){idMap[$2->cNode[0]->value]->allocate($1->description);}
        else if ($2->cNode[0]->description == "array_id"){idMap[$2->cNode[0]->cNode[0]->value]->allocate($1->description);}
        }
    ;

    declaration_function: var_type ID LP paramester_list RP LBRACE statement RBRACE {
        $$ = new node("declaration_function", new(node*[4]){$1, $2, $4, $7}, 4);}
    ;

    paramester_list: paramester COMMA paramester_list {
        int cNodeLength = 0;
        if($3 != 0){cNodeLength = $3->cNodeLength;}
    $$ = new node("paramester_list", parserTree.unit_node($1, $3), cNodeLength + 1);}
    | paramester {
        $$ = new node("paramester_list", new(node*[1]){$1}, 1);}
    | {$$ = 0;}
    ;
    paramester: var_type ID {
        $$ = new node("paramester", new(node*[2]){$1, $2}, 2);}
    ;

    declaration_struct: STRUCT ID LBRACE declaration_var_list RBRACE {
        $$ = new node("declaration_struct", new(node*[2]){$2, $4}, 2);}
    ;

    declaration_var_list: declaration_var SEMICOLON declaration_var_list {
        int cNodeLength = 0;
        if($3 != 0){cNodeLength = $3->cNodeLength;}
        $$ = new node("declaration_var_list", parserTree.unit_node($1, $3), cNodeLength + 1);}
    | {$$ = 0;}
    ;
    statement: single_statement statement {
        int cNodeLength = 0;
        if($2 != 0){cNodeLength = $2->cNodeLength;}
        $$ = new node("statement", parserTree.unit_node($1, $2), cNodeLength + 1);}
    | control_statement statement {
        int cNodeLength = 0;
        if($2 != 0){cNodeLength = $2->cNodeLength;}
        $$ = new node("statement", parserTree.unit_node($1, $2), cNodeLength + 1);}
    | {$$ = 0;}
    ;

    control_statement: conditional_statement {$$ = $1;}
    | loop_statement {$$ = $1;}
    ;

    single_statement: init_var SEMICOLON {$$ = $1;}
    | expression SEMICOLON {$$ = $1;}
    | function_expression SEMICOLON {$$ = $1;}
    | declaration_struct SEMICOLON {$$ = $1;}
    ;

    function_expression: CONTINUE {$$ = new node("CONTINUE", 0, 0);}
    | BREAK {$$ = new node("BREAK", 0, 0);}
    | RETURN {$$ = new node("RETURN", 0, 0);}
    | RETURN expression {$$ = new node("RETURN", new(node*[1]){$2}, 1);}
    ;

    conditional_statement: if_statement {
        $$ = new node("conditional_statement", new(node*[1]){$1}, 1);}
    | if_statement eles_statement {
        $$ = new node("conditional_statement", new(node*[2]){$1, $2}, 2);}
    | switch_statement {
        $$ = new node("conditional_statement", new(node*[1]){$1}, 1);}
    ;
    if_statement: IF LP logical_or_expression RP LBRACE statement RBRACE {
        $$ = new node("if_statement", new(node*[2]){$3, $6}, 2);}
    | IF LP logical_or_expression RP single_statement {
        $$ = new node("if_statement", new(node*[2]){$3, $5}, 2);}
    | IF LP logical_or_expression RP control_statement {
        $$ = new node("if_statement", new(node*[2]){$3, $5}, 2);}
    ;
    eles_statement: ELSE LBRACE statement RBRACE {
        $$ = new node("else_statement", new(node*[1]){$3}, 1);}
    | ELSE single_statement {
        $$ = new node("else_statement", new(node*[1]){$2}, 1);}
    | ELSE control_statement {
        $$ = new node("else_statement", new(node*[1]){$2}, 1);}
    ;
    switch_statement: SWITCH LP expression RP LBRACE switch_match RBRACE {
        $$ = new node("switch_statement", new(node*[2]){$3, $6}, 2);}
    ;
    switch_match: case_list {$$ = new node("switch_match", new(node*[1]){$1}, 1);}
    | case_list default_case {$$ = new node("switch_match", new(node*[2]){$1, $2}, 2);}
    ;
    case_list: single_case case_list {int cNodeLength = 0;
        if($2 != 0){cNodeLength = $2->cNodeLength;}
        $$ = new node("case_list", parserTree.unit_node($1, $2), cNodeLength + 1);}
    | {$$ = 0;}
    ;
    single_case: CASE NUMBER COLON statement {$$ = new node("single_case", new(node*[2]){$2, $4}, 2);}
    ;
    default_case: DEFAULT COLON statement {$$ = new node("default_case", new(node*[1]){$3}, 1);}
    ;

    loop_statement: for_statememt {$$ = $1;}
    | while_statement {$$ = $1;}
    | do_while_statement {$$ = $1;}
    ;
    for_statememt: FOR LP single_statement logical_or_expression SEMICOLON expression RP LBRACE statement RBRACE {
        $$ = new node("for_statement", new(node*[4]){$3, $4, $6, $9}, 4);}
    ;
    while_statement: WHILE LP logical_or_expression RP LBRACE statement RBRACE {
        $$ = new node("while_statement", new(node*[2]){$3, $6}, 2);}
    ;
    do_while_statement: DO LBRACE statement RBRACE WHILE LP logical_or_expression RP SEMICOLON {
        $$ = new node("do_while_statement", new(node*[2]){$3, $7}, 2);}
    ;

    var_type: INT {$$ = new node("int", 0, 0);}
    | DOUBLE {$$ = new node("double", 0, 0);}
    | FLOAT {$$ = new node("float", 0, 0);}
    | STRING {$$ = new node("string", 0, 0);}
    | LONG {$$ = new node("long", 0, 0);}
    | SHORT {$$ = new node("short", 0, 0);}
    | BYTE {$$ = new node("byte", 0, 0);}
    | VOID {$$ = new node("void", 0, 0);}
    | STRUCT ID {$$ = new node("struct", new(node*[1]){$2}, 1);}
    ;

    expression: assignment_expression {$$ = $1;}
    | logical_or_expression {$$ = $1;}
    | array_expression {$$ = $1;}
    | function_call_expression {$$ = $1;}
    | logical_or_expression COMMA expression {$$ = new node(",", new(node*[2]){$1, $3}, 2);}
    | assignment_expression COMMA expression {$$ = new node(",", new(node*[2]){$1, $3}, 2);}
    ;

    function_call_expression: ID LP argv_body RP {$$ = new node("function_call_expression", new(node*[2]){$1, $3}, 2);}
    ;

    array_expression: LBRACE argv_body RBRACE {$$ = new node("array_expression", new(node*[1]){$2}, 1);}
    ;

    argv_body: argv_list {$$ = $1;}
    | {$$ = 0;}
    ;

    argv_list: array_element COMMA argv_list {
        int cNodeLength = 0;
        if($3 != 0){cNodeLength = $3->cNodeLength;}
        $$ = new node("argv_list", parserTree.unit_node($1, $3), cNodeLength + 1);}
    | array_element {$$ = new node("argv_list", new(node*[1]){$1}, 1);}
    ;

    array_element: declaration_expression {$$ = new node("array_element", new(node*[1]){$1}, 1);}
    ;

    assignment_expression: var ASSIGN expression {
        $$ = new node("=", new(node*[2]){$1, $3}, 2);
        }
    | var MUL_ASSIGN expression {$$ = new node("*=", new(node*[2]){$1, $3}, 2);}
    | var DIV_ASSIGN expression {$$ = new node("/=", new(node*[2]){$1, $3}, 2);}
    | var MOD_ASSIGN expression {$$ = new node("%=", new(node*[2]){$1, $3}, 2);}
    | var ADD_ASSIGN expression {$$ = new node("+=", new(node*[2]){$1, $3}, 2);}
    | var SUB_ASSIGN expression {$$ = new node("-=", new(node*[2]){$1, $3}, 2);}
    | var LSHIFT_ASSIGN expression {$$ = new node(">>=", new(node*[2]){$1, $3}, 2);}
    | var RSHIFT_ASSIGN expression {$$ = new node("<<=", new(node*[2]){$1, $3}, 2);}
    | var AND_ASSIGN expression {$$ = new node("&=", new(node*[2]){$1, $3}, 2);}
    | var OR_ASSIGN expression {$$ = new node("|=", new(node*[2]){$1, $3}, 2);}
    | var XOR_ASSIGN expression {$$ = new node("^=", new(node*[2]){$1, $3}, 2);}  
    ;
    
    logical_or_expression: logical_and_expression {$$ = $1;}
    | logical_or_expression OR logical_and_expression {$$ = new node("||", new(node*[2]){$1, $3}, 2);}
    ;
    
    logical_and_expression: inclusive_or_expression {$$ = $1;}
    | logical_and_expression AND inclusive_or_expression {$$ = new node("&&", new(node*[2]){$1, $3}, 2);}
    ;

    
    inclusive_or_expression: inclusive_and_expression {$$ = $1;}
    | inclusive_or_expression BITWISEOR inclusive_and_expression {$$ = new node("|", new(node*[2]){$1, $3}, 2);}
    ;
    
    inclusive_and_expression: equality_expression {$$ = $1;}
    | inclusive_and_expression BITWISEADD equality_expression {$$ = new node("&", new(node*[2]){$1, $3}, 2);}
    ;
    
    equality_expression: relational_expression {$$ = $1;}
    | equality_expression EQ relational_expression {$$ = new node("==", new(node*[2]){$1, $3}, 2);}
    | equality_expression NZ relational_expression {$$ = new node("!=", new(node*[2]){$1, $3}, 2);}
    ;
    
    relational_expression: shift_expression {$$ = $1;}
    | relational_expression LT shift_expression {$$ = new node("<", new(node*[2]){$1, $3}, 2);}
    | relational_expression GT shift_expression {$$ = new node(">", new(node*[2]){$1, $3}, 2);}
    | relational_expression LE shift_expression {$$ = new node("<=", new(node*[2]){$1, $3}, 2);}
    | relational_expression GE shift_expression {$$ = new node(">=", new(node*[2]){$1, $3}, 2);}
    ;
        
    shift_expression: additive_expression {$$ = $1;}
    | shift_expression LSHIFT additive_expression {$$ = new node(">>", new(node*[2]){$1, $3}, 2);}
    | shift_expression RSHIFT additive_expression {$$ = new node("<<", new(node*[2]){$1, $3}, 2);}
    ;
    
    additive_expression: multiplicative_expression {$$ = $1;}
    | additive_expression ADD multiplicative_expression {
        $$ = new node("+", new(node*[2]){$1, $3}, 2);
        }
    | additive_expression SUB multiplicative_expression {
        $$ = new node("-", new(node*[2]){$1, $3}, 2);
        }
    ;
    
    multiplicative_expression: power_expression {$$ = $1;}
    | multiplicative_expression MUL power_expression {
        $$ = new node("*", new(node*[2]){$1, $3}, 2);
        }
    | multiplicative_expression DIV power_expression {
        $$ = new node("/", new(node*[2]){$1, $3}, 2);
        }
    | multiplicative_expression MOD power_expression {
        $$ = new node("%", new(node*[2]){$1, $3}, 2);
        }
    ;
    
    power_expression: cast_expression {$$ = $1;}
    | power_expression POW cast_expression {
        $$ = new node("^", new(node*[2]){$1, $3}, 2);
        }
    ;
    
    cast_expression: unary_expression {$$ = $1;}
    | var_type LP cast_expression RP {$$ = new node("(type)", new(node*[2]){$1, $3}, 2);}
    ;
    
    unary_expression: declaration_expression {$$ = $1;}
    | INC var {$$ = new node("l_++", new(node*[1]){$2}, 1);}
    | DEC var {$$ = new node("l_--", new(node*[1]){$2}, 1);}
    | var INC {$$ = new node("r_++", new(node*[1]){$1}, 1);}
    | var DEC {$$ = new node("r_--", new(node*[1]){$1}, 1);}
    | LNOT var {$$ = new node("!", new(node*[1]){$2}, 1);}
    | LNOT parenthesized_expression {$$ = new node("!", new(node*[1]){$2}, 1);}
    ;

    declaration_expression: parenthesized_expression {$$ = $1;}
    | NUMBER {$$ = $1;}
    | var {$$ = $1;}
    | STR {$$ = $1;}
    ;

    var: ID dimension_list {
        if($2 == 0) {$$ = $1;}
        else {$$ = new node("array_id", new(node*[2]){$1, $2}, 2);}}
    | ID POINT ID {$$ = new node("struct_var", new(node*[2]){$1, $3}, 2);}
    | pointer_var {$$ = new node("pointer", new(node*[1]){$1}, 1);}
    ;

    dimension_list: dimension dimension_list {
        int cNodeLength = 0;
        if($2 != 0){cNodeLength = $2->cNodeLength;}
        $$ = new node("dimension_list", parserTree.unit_node($1, $2), cNodeLength + 1);}
    | {$$ = 0;}
    ;

    dimension: LSBRACKET RSBRACKET {$$ = new node("[]", 0, 0);}
    | LSBRACKET NUMBER RSBRACKET {$$ = $2;}
    | LSBRACKET ID RSBRACKET {$$ = $2;}
    ;

    pointer_var: MUL var {$$ = $2;}
    | MUL LP var RP {$$ = $3;}
    ;

    parenthesized_expression: LP expression RP {$$ = $2;}
    ;


%%

/////////////////////////////////////////////////////////////////////////////
// programs section
lexer lexer;
parser parser;

void parser::yyerror(const char YYFAR* text)
{
    cout<< "Line " << lexer.yylineno << ": syntax error near " << lexer.yytext << endl;
}

int main(void)
{
    string sFile;
    getline(cin, sFile);
    lexer.yyin = new std::ifstream(sFile);
    if(!(*lexer.yyin))
    {
        cout << "File doesn't exist." << endl;
        return 0;
    }
    int n = 1;
    if (parser.yycreate(&lexer)) {
        if (lexer.yycreate(&parser)) {
            n = parser.yyparse();
        }
    }
    parserTree.print(parserTree.root, "");
    parserList.idMap = idMap;
    parserList.generate(parserTree.root);
    parserList.print();
    return n;
}
