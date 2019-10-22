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
%}
%token ASSIGN EQ GT LT
%token GE LE NZ ADD SUB
%token MUL DIV MOD POW INC DEC
%token BITWISEOR BITWISEADD LSHIFT
%token RSHIFT AND OR LNOT LBRACE RBRACE LP
%token RP LSBRACKET RSBRACKET SEMICOLON
%token COMMA POINT SQUITO DQUITO
%token NOTE NUMBER ERROR ID STR

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
#define YYSTYPE int
#endif
}

// place any declarations here

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

// place your YACC rules here (there must be at least one)

Grammar
	: /* empty */
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
	return n;
}

