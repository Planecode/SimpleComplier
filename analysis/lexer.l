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
EQ ==
GT >
LT <
GE >=
LE <=
NZ !=

ADD +
SUB -
MUL *
DIV /
MOD %
POW ^

ADD &&
OR ||
NOT !

LBRACE {
RBRACE }
LP (
RP )
LSBRACKET [
RSBRACKET ]
SEMICOLON ;
SQUITO '
DQUITO "
NOTE //
LNOTE /*
RNOTE */

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
{EQ}     {printf("EQ        ==\n");}
{GT}     {printf("GT        >\n");}
{LT}     {printf("LT        <\n");}
{GE}     {printf("GE        >=\n");}
{LE}     {printf("LE        <=\n");}
{NZ}     {printf("NZ        !=\n");}

{ADD}      {printf("ADD      +\n");}
{SUB}      {printf("SUB     -\n");}
{MUL}      {printf("MUL       *\n");}
{DIV}      {printf("DIV       /\n");}
{MOD}      {printf("MOD       %\n");}
{POW}      {printf("POW       ^\n");}

{AND}     {printf("AND       &&\n");}
{OR}      {printf("OR        ||\n");}
{NOT}     {printf("NOT   !\n");}


{LBRACE}       {printf("LBRACE    {\n");}
{RBRACE}       {printf("RBRACE    }\n");}
{LP}           {printf("LP    (\n");}
{RP}           {printf("RP    )\n");}
{LSBRACKET}    {printf("LSBRACKET [\n");}
{RSBRACKET}    {printf("RSBRACKET ]\n");}
{SEMICOLON}    {printf("SEMICOLON ;\n");}
{SQUITO}       {printf("SQUITO    '\n");}
{DQUITO}       {printf("DQUITO    \"\n");}
{NOTE}         {printf("NOTE      \//\n");}
{LNOTE}        {printf("LNOTE     \/*\n");}
{RNOTE}        {printf("RNOTE     */\n");}



{delim} {}
{number} {printf("NUMBER:%s\n", yytext);}
{id} {printf("ID:%s\n", yytext);}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section