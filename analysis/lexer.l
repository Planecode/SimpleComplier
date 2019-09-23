%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.
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
void void
int int
double double
float float
char char
string string
long long
short short
byte byte

assign =

delim [ \n\r\t]+
number (\+|\-)?[1-9][0-9]*(.[0-9]*)?((e|E)(\+|\-)?[0-9]*)?
id (_|[a-zA-Z0-9])(_|[a-zA-Z0-9])*

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

%{
	// extract yylval for use later on in actions
	YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
%}

// place your Lex rules here
{void} {printf("VOID\n");}
{int} {printf("INT\n");}
{double} {printf("DOUBLE\n");}
{float} {printf("FLOAT\n");}
{char} {printf("CHAR\n");}
{long} {printf("LONG\n");}
{short} {printf("SHORT\n");}
{byte} {printf("BYTE\n");}

{assign} {printf("ASSIGN\n");}

{delim} {}
{number} {printf("NUMBER:%s\n", yytext);}
{id} {printf("ID:%s\n", yytext);}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section

