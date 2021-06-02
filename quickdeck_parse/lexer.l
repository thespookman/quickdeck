%option noyywrap nounput case-insensitive
%{
#include <cstdlib>

#include "parser.h"
#include "parser.tab.hh"

#define YY_DECL int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type *yylloc)

int yycolumn=1;

#define YY_USER_ACTION yylloc->initialize(NULL, yylineno, yycolumn); yylloc->columns(yyleng); yycolumn += yyleng;

typedef yy::parser::token token;
%}
%%

%{
yylloc->step();
%}

"{" 	|
"}" 	|
"=" 	|
";" 	|
"," 	|
"(" 	|
")" 	{ return yytext[0]; }

"if"	{ return token::IF; }
"else"	{ return token::ELSE; }
"!"		{ return token::NOT; }
"+"		{ return token::PLUS; }
"-"		{ return token::MINUS; }
"*"		{ return token::MULTIPLY; }
"/"		{ return token::DIVIDE; }
"=="		{ return token::EQUAL; }
"!="		{ return token::NOT_EQUAL; }
">"		{ return token::GREATER; }
"<"		{ return token::LESS; }
">="		{ return token::GREATER_EQUAL; }
"<="		{ return token::LESS_EQUAL; }
"||"		{ return token::OR; }
"&&"		{ return token::AND; }

"echo"		{ yylval->fn = FUNC_ECHO; return token::FUNCTION; }
"new"		{ yylval->fn = FUNC_NEW; return token::FUNCTION; }
"save"		{ yylval->fn = FUNC_SAVE; return token::FUNCTION; }
"rect"		{ yylval->fn = FUNC_RECT; return token::FUNCTION; }
"text"		{ yylval->fn = FUNC_TEXT; return token::FUNCTION; }
"image"		{ yylval->fn = FUNC_IMAGE; return token::FUNCTION; }
"line"		{ yylval->fn = FUNC_LINE; return token::FUNCTION; }

"true"		{ yylval->b = true; return token::BOOLEAN; }
"false" 	{ yylval->b = false; return token::BOOLEAN; }
[0-9]*"."?[0-9]+	{ yylval->d = atof(yytext); return token::NUMBER; }
[_a-z][_a-z0-9]*		{ yylval->c = strdup(yytext); return token::VARIABLE; }
\"(\\\"|[^"\n])*\"	{ yylval->c = strdup(yytext+1); yylval->c[yyleng-2] = 0; return token::STRING; }

<<EOF>>	{ return 0; }

"//".*	|
[ \t]	{ yylloc->step(); }
\n	{ ++yylineno; yycolumn = 1; }

.	{ yylloc->step(); }

%%

