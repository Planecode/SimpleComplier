%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.

Date: 2019Äê9ÔÂ23ÈÕ
****************************************************************************/

#include "parser.h"
%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// lexical analyser name
%name lexer

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

// place any declarations here

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

%{
	// extract yylval for use later on in actions
	YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
%}

// place your Lex rules here

%%

/////////////////////////////////////////////////////////////////////////////
// programs section

