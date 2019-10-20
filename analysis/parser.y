%{
/****************************************************************************
parser.y
ParserWizard generated YACC file.
****************************************************************************/

#include "lexer.h"
#include "fstream"
#include "iostream"
#include "string"
using namespace std;
%}

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

