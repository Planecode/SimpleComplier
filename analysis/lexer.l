%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.
****************************************************************************/

#include "parser.h"
#include "stdio.h"
int install_id(char* yytext);
int install_num(char* yytext);
int install_str(char* yytext);
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
gt \>
lt \<
ge \>=
le \<=
nz !=

add \+
sub \-
mul \*
div \/
mod \%
pow \^

and \&\&
or \|\|
not \!

lbrace \{
rbrace \}
lp \(
rp \)
lsbracket \[
rsbracket \]
semicolon \;
squito \'
dquito \"
note \/\/
lnote \/\*
rnote \*\/

delim [ \n\r\t]+
number (\+|\-)?[1-9][0-9]*(.[0-9]*)?((e|E)(\+|\-)?[0-9]*)?
id (_|[a-zA-Z0-9])(_|[a-zA-Z0-9])*
str ((\").*(\"))|((\').*(\'))

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

%{
	// extract yylval for use later on in actions
	YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
%}

// place your Lex rules here
{void} {printf("VOID\n");}
{int} {printf("INT %s\n", yytext);}
{double} {printf("DOUBLE %s\n", yytext);}
{float} {printf("FLOAT %s\n", yytext);}
{char} {printf("CHAR %s\n", yytext);}
{long} {printf("LONG %s\n", yytext);}
{short} {printf("SHORT %s\n", yytext);}
{byte} {printf("BYTE %s\n", yytext);}

{assign} {printf("ASSIGN\n");}
{eq}     {printf("EQ ==\n");}
{gt}     {printf("GT        >\n");}
{lt}     {printf("LT        <\n");}
{ge}     {printf("GE        >=\n");}
{le}     {printf("LE        <=\n");}
{nz}     {printf("NZ        !=\n");}

{add}      {printf("ADD      +\n");}
{sub}      {printf("SUB     -\n");}
{mul}      {printf("MUL       *\n");}
{div}      {printf("DIV       /\n");}
{mod}      {printf("MOD       %%\n");}
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
{note}         {printf("NOTE      //n");}
{lnote}        {printf("LNOTE     /*\n");}
{rnote}        {printf("RNOTE     */\n");}

{delim} {}
{number} {printf("NUMBER %s %d\n", yytext, install_num(yytext));}
{id} {printf("ID %s %d\n", yytext, install_id(yytext));}
{str} {printf("String %s %d\n", yytext, install_str(yytext));}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section
int install_num(char* yytext)
{
	return atoi(yytext);
}

int install_id(char* yytext)
{
	return int(new string(yytext));
}

int install_str(char* yytext)
{
	string str = yytext;
	str = str.substr(1, str.length()-2);
	return int(new string(str));
}
