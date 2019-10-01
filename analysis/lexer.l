%{
/****************************************************************************
lexer.l
ParserWizard generated Lex file.
****************************************************************************/

#include "parser.h"
#include "stdio.h"
#include "map"
#include "set"
#include "algorithm"
int install_id(char* yytext);
int install_num(char* yytext);
int install_str(char* yytext);
string get_token(char* yytext);
void strtoupper(string& str);
map<string, int> idMap;
set<string> wordSet{"void", "int", "double", "float", "string", "long", 
"short", "byte", "if", "else", "for", "switch", "main", "continue", 
"break", "case", "default", "while", "new", "return"};
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
inc \+\+
dec \-\-


bitwiseadd \&
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
squito \'
dquito \"
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
{assign} {printf("ASSIGN %s\n", yytext);}
{eq} {printf("EQ %s\n", yytext);}
{gt} {printf("GT %s\n", yytext);}
{lt} {printf("LT %s\n", yytext);}
{ge} {printf("GE %s\n", yytext);}
{le} {printf("LE %s\n", yytext);}
{nz} {printf("NZ %s\n", yytext);}

{add} {printf("ADD %s\n", yytext);}
{sub} {printf("SUB %s\n", yytext);}
{mul} {printf("MUL %s\n", yytext);}
{div} {printf("DIV %s\n", yytext);}
{mod} {printf("MOD %s\n", yytext);}
{pow} {printf("POW %s\n", yytext);}
{inc} {printf("INC %s\n", yytext);}
{dec} {printf("DEC %s\n", yytext);}


{bitwiseadd} {printf("BITWISEADD %s\n", yytext);}
{and} {printf("AND %s\n", yytext);}
{or} {printf("OR %s\n", yytext);}
{not} {printf("LNOT %s\n", yytext);}


{lbrace} {printf("LBRACE %s\n", yytext);}
{rbrace} {printf("RBRACE %s\n", yytext);}
{lp} {printf("LP %s\n", yytext);}
{rp} {printf("RP %s\n", yytext);}
{lsbracket} {printf("LSBRACKET %s\n", yytext);}
{rsbracket} {printf("RSBRACKET %s\n", yytext);}
{semicolon} {printf("SEMICOLON %s\n", yytext);}
{comma} {printf("COMMA %s\n", yytext);}
{point} {printf("POINT %s\n", yytext);}
{squito} {printf("SQUITO %s\n", yytext);}
{dquito} {printf("DQUITO %s\n", yytext);}
{note} {printf("NOTE %s\n", yytext);}

{delim} {}
{number} {printf("NUMBER %s %s\n", yytext, yytext);}
{id} {string token = get_token(yytext);if(token == "ID"){printf("ID %s %d\n", yytext, install_id(yytext));}else{printf("%s %s\n",token.c_str(), yytext);};}
{str} {printf("String %s %d\n", yytext, install_str(yytext));}
{error} {printf("ERROR %s\n", yytext);}

%%

/////////////////////////////////////////////////////////////////////////////
// programs section
void strtoupper(string& str){
    int len=str.size();
    for(int i=0;i<len;i++){
        if(str[i]>='a'&&str[i]<='z'){
            str[i]-=32;
        }
    }
}

string get_token(char* yytext)
{
	string word = yytext;
	if(wordSet.find(word) != wordSet.end())
	{
		strtoupper(word);
		return word;
	}
		
	return "ID";
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
