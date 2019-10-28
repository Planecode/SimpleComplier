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
    | statement_list {$$ = $1;}
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
    | {$$ = 0;}
    ;

    conditional_statement: if_statement {$$ = new node("conditional_statement", 0, new(node*[1]){$1}, 1);}
    | if_statement eles_statement {$$ = new node("conditional_statement", 0, new(node*[2]){$1, $2}, 2);}
    ;
    if_statement: IF LP RP LBRACE statement RBRACE {$$ = new node("if_statement", 0, new(node*[1]){$5}, 1);}
    ;
    eles_statement: ELSE LBRACE statement RBRACE {$$ = new node("else_statement", 0, new(node*[1]){$3}, 1);}
    ;

    loop_statement: for_statememt {$$ = $1;}
    | while_statement {$$ = $1;}
    ;
    for_statememt: FOR LP RP LBRACE statement RBRACE {$$ = new node("for_statement", 0, new(node*[1]){$5}, 1);}
    ;
    while_statement: WHILE LP RP LBRACE statement RBRACE {$$ = new node("while_statement", 0, new(node*[1]){$5}, 1);}
    ;

    var_type: INT {$$ = new node("int", 0, 0, 0);}
    ;
    assign: ASSIGN {$$ = new node("=", 0, 0, 0);}
    ;
    
    statement_list: _statement statement_list {$$ = new node("statement_list", 0, new(node*[2]){$1, $2}, 2);}
	| {$$ = 0;}
	;
	_statement: expression SEMICOLON {$$ = new node("statement", 0, new(node*[1]){$1}, 1);}
	;
	
	expression
	: assignment_expression {$$ = $1;}
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
	| postfix_expression {$$ = $1;}
	| primary_expression {$$ = $1;}
	| assignment_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| logical_or_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| logical_and_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| inclusive_or_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| and_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| equality_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| relational_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| shift_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| additive_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| multiplicative_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| power_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| cast_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| unary_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| postfix_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| primary_expression comma expression {$$ = new node("expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	comma: COMMA {$$ = new node(" , ", 0, 0, 0);}
	;
	
	assignment_expression        //��ֵ�������16����
	: unary_expression assign assignment_expression {$$ = new node("assignment_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| {$$ = 0;}
	;
	//assignment_operator
	//: '='
	//| MUL_ASSIGN
	//| DIV_ASSIGN
	//| MOD_ASSIGN
	//| ADD_ASSIGN
	//| SUB_ASSIGN
	//| LEFT_ASSIGN
	//| RIGHT_ASSIGN
	//| AND_ASSIGN
	//| XOR_ASSIGN
	//| OR_ASSIGN
	//;
	
	//conditional_expression       //��Ԫ�����������15����
	//: logical_or_expression
	//| logical_or_expression '?' expression ':' conditional_expression
	//;
	//conditional_expression
	//;

	
	logical_or_expression       //�߼���14����
	: logical_and_expression {$$ = $1;}
	| logical_or_expression or logical_and_expression {$$ = new node("logical_or_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	or: OR {$$ = new node(" || ", 0, 0, 0);}
	;
	
	logical_and_expression      //�߼��루13����
	: inclusive_or_expression {$$ = $1;}
	| logical_and_expression and inclusive_or_expression {$$ = new node("logical_and_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	and: AND {$$ = new node(" && ", 0, 0, 0);}
	;
	
	inclusive_or_expression     //λ��12����
	: and_expression {$$ = $1;}
	| inclusive_or_expression bitwiseor and_expression {$$ = new node("inclusive_or_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	bitwiseor: BITWISEOR {$$ = new node(" | ", 0, 0, 0);}
	;
	
	//exclusive_or_expression     //λ���11����
	//: and_expression {$$ = $1;}
	//| exclusive_or_expression '^' and_expression {$$ = new node("exclusive_or_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	//;
	
	and_expression              //λ�루10����
	: equality_expression {$$ = $1;}
	| and_expression bitwiseadd equality_expression {$$ = new node("and_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	
	bitwiseadd: BITWISEADD {$$ = new node(" & ", 0, 0, 0);}
	;
	
	equality_expression          //����������9����
	: relational_expression {$$ = $1;}
	| equality_expression eq relational_expression {$$ = new node("equality_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| equality_expression nz relational_expression {$$ = new node("equality_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	eq: EQ {$$ = new node(" == ", 0, 0, 0);}
	;
	nz: NZ {$$ = new node(" != ", 0, 0, 0);}
	;
	
	relational_expression        //��ϵ�������8����
	: shift_expression {$$ = $1;}
	| relational_expression lt shift_expression {$$ = new node("relational_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| relational_expression gt shift_expression {$$ = new node("relational_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| relational_expression le shift_expression {$$ = new node("relational_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| relational_expression ge shift_expression {$$ = new node("relational_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	gt: GT {$$ = new node(" > ", 0, 0, 0);}
	;
	lt: LT {$$ = new node(" < ", 0, 0, 0);}
	;
	ge: GE {$$ = new node(" >= ", 0, 0, 0);}
	;
	le: LE {$$ = new node(" <= ", 0, 0, 0);}
	;
	
	shift_expression              //��λ�������7����
	: additive_expression {$$ = $1;}
	| shift_expression lshift additive_expression {$$ = new node("shift_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| shift_expression rshift additive_expression {$$ = new node("shift_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	lshift: LSHIFT {$$ = new node(" << ", 0, 0, 0);}
	;
	rshift: RSHIFT {$$ = new node(" >> ", 0, 0, 0);}
	;
	
	additive_expression           //�ӷ����ͱ��ʽ��6����
	: multiplicative_expression {$$ = $1;}
	| additive_expression add multiplicative_expression {$$ = new node("additive_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| additive_expression sub multiplicative_expression {$$ = new node("additive_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	add: ADD {$$ = new node(" + ", 0, 0, 0);}
	;
	sub: SUB {$$ = new node(" - ", 0, 0, 0);}
	;
	
	multiplicative_expression     //�˷����ʽ��5����
	: power_expression {$$ = $1;}
	| multiplicative_expression mul power_expression {$$ = new node("multiplicative_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| multiplicative_expression div power_expression {$$ = new node("multiplicative_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	| multiplicative_expression mod power_expression {$$ = new node("multiplicative_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	mul: MUL {$$ = new node(" * ", 0, 0, 0);}
	;
	div: DIV {$$ = new node(" / ", 0, 0, 0);}
	;
	mod: MOD {$$ = new node(" % ", 0, 0, 0);}
	;
	
	power_expression
	: cast_expression {$$ = $1;}
	| power_expression pow cast_expression {$$ = new node("power_expression", 0, new(node*[3]){$1,$2,$3}, 3);}
	;
	pow: POW {$$ = new node(" ^ ", 0, 0, 0);}
	;
	
	cast_expression              //ǿ�����ͱ��ʽ��4����
	: unary_expression {$$ = $1;}
	| lp var_type rp cast_expression {$$ = new node("cast_expression", 0, new(node*[4]){$1,$2,$3,$4}, 4);}
	;
	
	lp: LP {$$ = new node(" ( ", 0, 0, 0);}
	;
	rp: RP {$$ = new node(" ) ", 0, 0, 0);}
	;
	
	unary_expression                //һԪ���ʽ��3����
	: postfix_expression {$$ = $1;}
	| inc unary_expression {$$ = new node("unary_expression", 0, new(node*[2]){$1,$2}, 2);}
	| dec unary_expression {$$ = new node("unary_expression", 0, new(node*[2]){$1,$2}, 2);}
	| not cast_expression {$$ = new node("unary_expression", 0, new(node*[2]){$1,$2}, 2);}  //��Ŀת�ͱ��ʽ��&��*��+��-��~��!
	//| SIZEOF unary_expression
	//| SIZEOF '(' type_name ')'
	;
	not: LNOT {$$ = new node(" ! ", 0, 0, 0);}
	;
	
	postfix_expression               //��׺���ʽ��2����                        
	: primary_expression {$$ = $1;}
	//| postfix_expression '[' expression ']'                 //����
	//| postfix_expression '(' ')'                            //�������޲�����
	//| postfix_expression '(' argument_expression_list ')'   //����
	//| postfix_expression '.' IDENTIFIER                     //��Ա
	//| postfix_expression '->' IDENTIFIER                    //��Ա
	| postfix_expression inc {$$ = new node("postfix_expression", 0, new(node*[2]){$1,$2}, 2);}      //�Լ�
	| postfix_expression dec {$$ = new node("postfix_expression", 0, new(node*[2]){$1,$2}, 2);}      //�Լ�
	;
	inc: INC {$$ = new node(" ++ ", 0, 0, 0);}
	;
	dec: DEC {$$ = new node(" -- ", 0, 0, 0);}
	;
	//argument_expression_list       //�����Ĳ�����
	//: assignment_expression
	//| argument_expression_list ',' assignment_expression
	//;
	
	primary_expression               //�������ʽ��1����
	: ID {$$ = $1;}
	| NUMBER {$$ = $1;}
	| STR {$$ = $1;}
	//| CONSTANT
	//| STRING_LITERAL
	| lp expression rp {$$ = new node("primary_experssion", 0, new(node*[3]){$1, $2, $3}, 3);}
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

