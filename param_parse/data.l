%option noyywrap nounput case-insensitive
%{
#include <cstdlib>

#include "data.h"
#include "data.tab.hh"

#define YY_DECL int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type *yylloc)

int yycolumn=1;

#define YY_USER_ACTION yylloc->initialize(NULL, yylineno, yycolumn); yylloc->columns(yyleng); yycolumn += yyleng;

typedef yy::parser::token token;
%}
%%

%{
yylloc->step();
%}
"," 			{ return token::DELIM; } 

"true"          	{ yylval->b = true; return token::BOOLEAN; }
"false"         	{ yylval->b = false; return token::BOOLEAN; }
[0-9]*"."?[0-9]+        { yylval->d = atof(yytext); return token::NUMBER; }
[_a-z][_a-z0-9]*  	{ yylval->c=strdup(yytext); return token::VARIABLE; }
\"(\\\"|[^"\n])*\"      { yylval->c=strdup(yytext); return token::STRING; }

[ \t]			{ yylloc->step(); }
\n			{ ++yylineno; yycolumn = 1; return token::ENDL; }

<<EOF>>			{ return 0; }

%%

