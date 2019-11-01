%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.
****************************************************************************/

#include <cstdio>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <fstream>
#include "parser.h"
#include "tree.h"
extern int yylval;
using namespace std;
int install_id(char* yytext);
int install_num(char* yytext);
int install_str(char* yytext);
int get_token(char* yytext);
map<string, int> idMap;
map<string, int> wordMap{{"void", VOID}, {"int", INT}, {"double", DOUBLE}, {"float", FLOAT}, {"string", STRING}, {"long", LONG}, 
{"short", SHORT}, {"byte", BYTE}, {"if", IF}, {"else", ELSE}, {"for", FOR}, {"switch", SWITCH}, {"main",MAIN}, {"continue", CONTINUE}, 
{"break", BREAK}, {"case", CASE}, {"default", DEFAULT}, {"while", WHILE}, {"do", DO}, {"new", NEW}, {"return", RETURN}, {"struct", STRUCT}};
%}

/////////////////////////////////////////////////////////////////////////////
// declarations section

// lexical analyser name
%name lexer

// class definition
{
virtual int yygetchar();
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

assign =
eq ==
gt \>
lt \<
ge \>=
le \<=
nz !=
mulassign \*\=
divassign \/\=
modassign \%\=
addassign \+\=
subassign \-\=
lshiftassign \<\<\=
rshiftassign \>\>\=
andassign \&\=
orassign \|\=
xorassign \^\=

add \+
sub \-
mul \*
div \/
mod \%
pow \^
inc \+\+
dec \-\-


bitwiseor \|
bitwiseadd \&
lshift \<\<
rshift \>\>

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
comma \,
point \.
colon \:
note (\/\/.*(\r|\n|(\r\n)))|(\/\*((\*+[^\*\/]+)*[^\*]*)*\*+\/)

delim [ \n\r\t]+
number (\+|\-)?0|([1-9][0-9]*(\.[0-9]+)?((e|E)(\+|\-)?[0-9]+)?)
id (_|[a-zA-Z])(_|[a-zA-Z0-9])*
str ((\").*(\"))|((\').*(\'))
error ([_a-zA-Z0-9]*)|.

%%

/////////////////////////////////////////////////////////////////////////////
// rules section

%{
    // extract yylval for use later on in actions
    YYSTYPE YYFAR& yylval = *(YYSTYPE YYFAR*)yyparserptr->yylvalptr;
%}

// place your Lex rules here
{assign} {return ASSIGN;}
{eq} {return EQ;}
{gt} {return GT;}
{lt} {return LT;}
{ge} {return GE;}
{le} {return LE;}
{nz} {return NZ;}
{mulassign} {return MUL_ASSIGN;}
{divassign} {return DIV_ASSIGN;}
{modassign} {return MOD_ASSIGN;}
{addassign} {return ADD_ASSIGN;}
{subassign} {return SUB_ASSIGN;}
{lshiftassign} {return LSHIFT_ASSIGN;}
{rshiftassign} {return RSHIFT_ASSIGN;}
{andassign} {return AND_ASSIGN;}
{orassign} {return OR_ASSIGN;}
{xorassign} {return XOR_ASSIGN;}


{add} {return ADD;}
{sub} {return SUB;}
{mul} {return MUL;}
{div} {return DIV;}
{mod} {return MOD;}
{pow} {return POW;}
{inc} {return INC;}
{dec} {return DEC;}


{bitwiseor} {return BITWISEOR;}
{bitwiseadd} {return BITWISEADD;}
{lshift} {return LSHIFT;}
{rshift} {return RSHIFT;}

{and} {return AND;}
{or} {return OR;}
{not} {return LNOT;}


{lbrace} {return LBRACE;}
{rbrace} {return RBRACE;}
{lp} {return LP;}
{rp} {return RP;}
{lsbracket} {return LSBRACKET;}
{rsbracket} {return RSBRACKET;}
{semicolon} {return SEMICOLON;}
{comma} {return COMMA;}
{point} {return POINT;}
{colon} {return COLON;}
{note} {}

{delim} {}
{number} {string value = yytext; yylval = new node("number: " + value, 0, 0, 0); return NUMBER;}
{id} {string value = yytext; int token = get_token(yytext);if(token == ID){yylval = new node("id: " + value, install_id(yytext), 0, 0);}return token;}
{str} {yylval=new node("str", install_str(yytext), 0, 0); return STR;}
{error} {return ERROR;}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section

int get_token(char* yytext)
{
    string word = yytext;
    if(wordMap.count(word) != 0)
    {
        return wordMap[word];
    }
    return ID;
}

int install_id(char* yytext)
{
    string id = yytext;
    if(idMap.count(id) == 0)
    {
        idMap[id] = int(new string(yytext));
    }
    return idMap[id];
}

int install_str(char* yytext)
{
    string str = yytext;
    str = str.substr(1, str.length()-2);
    if(idMap.count(str) == 0)
        idMap[str] = int(new string(yytext));
    return idMap[str];
}
int lexer::yygetchar()
{
    char c;
    yyin->get(c);
    if(yyin->eof())
    {
        return -1;
    }
    else
    {
        char * m_p = new char(c);
        return (unsigned char)*m_p;
    }
}
