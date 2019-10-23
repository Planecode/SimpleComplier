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
	program: declaration_group {parserTree.root = $1;}
	;
	declaration_group: declaration_block declaration_group {$$ = new node("declaration_group", 0, new(node*[2]){$1, $2}, 2);}
	| {$$ = 0;}
	;
	declaration_block: declaration_var {$$ = $1;}
	;
	declaration_var: var_type ID SEMICOLON {$$ = new node("declaration_var", 0, new(node*[2]){$1, $2}, 2);}
	| var_type ID assign NUMBER SEMICOLON {$$ = new node("declaration_var", 0, new(node*[4]){$1, $2, $3, $4}, 4);}
	;
	var_type: INT {$$ = new node(" int ", 0, 0, 0);}
	;
	assign: ASSIGN {$$ = new node(" = ", 0, 0, 0);}
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

