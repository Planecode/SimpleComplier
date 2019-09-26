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
eq ==
gt >
lt <
ge >=
le <=
nz !=

add +
sub -
mul *
div /
nod %
pow ^

and &&
or ||
not !

lbrace {
rbrace }
lp (
rp )
lsbpacket [
rsbracket ]
semicolon ;
squito '
dquito "
note //
lnote /*
rnote */

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
{eq}     {printf("EQ        ==\n");}
{gt}     {printf("GT        >\n");}
{lt}     {printf("LT        <\n");}
{ge}     {printf("GE        >=\n");}
{le}     {printf("LE        <=\n");}
{nz}     {printf("NZ        !=\n");}

{add}      {printf("ADD      +\n");}
{sub}      {printf("SUB     -\n");}
{mul}      {printf("MUL       *\n");}
{div}      {printf("DIV       /\n");}
{mod}      {printf("MOD       %\n");}
{pow}      {printf("POW       ^\n");}

{and}     {printf("AND       &&\n");}
{or}      {printf("OR        ||\n");}
{not}     {printf("NOT   !\n");}


{lbrace}       {printf("LBRACE    {\n");}
{rbrace}       {printf("RBRACE    }\n");}
{lp}           {printf("LP    (\n");}
{rp}           {printf("RP    )\n");}
{lsbracket}    {printf("LSBRACKET [\n");}
{rsbracket}    {printf("RSBRACKET ]\n");}
{semicolon}    {printf("SEMICOLON ;\n");}
{squito}       {printf("SQUITO    '\n");}
{dquito}       {printf("DQUITO    \"\n");}
{note}         {printf("NOTE      \//\n");}
{lnote}        {printf("LNOTE     \/*\n");}
{rnote}        {printf("RNOTE     */\n");}



{delim} {}
{number} {printf("NUMBER:%s\n", yytext);}
{id} {printf("ID:%s\n", yytext);}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section


