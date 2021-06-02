%language "C++"
%defines
%define parse.trace
%locations

%{
#include "ast.h"
#include "environment.h"
#include "operator.h"
#include "parser.h"
#include "value.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <stdexcept>

extern FILE *yyin;

Logger* l;
Environment* e;
%}

%union {
	Expression* e;
	Parameter* p;
	Statement* s;
	double d;
	bool b;
	char* c;
	int fn;
};

%token <d> NUMBER
%token <b> BOOLEAN
%token <c> STRING VARIABLE
%token <fn> FUNCTION
%token IF ELSE NOT PLUS MINUS MULTIPLY DIVIDE EQUAL NOT_EQUAL GREATER LESS GREATER_EQUAL LESS_EQUAL OR AND END_FILE ERROR

%nonassoc IF 
%right '='
%left ELSE
%left OR AND
%left EQUAL NOT_EQUAL GREATER LESS GREATER_EQUAL LESS_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE
%nonassoc NOT NEGATIVE

%type <e> expression
%type <p> parameter parameter_list
%type <s> statement statement_list

%destructor { l->dbg() << "Destructor called > "; l->dbg()<<"Delete at " << @$ << ": " << $$->describe() << "... "; delete $$; l->dbg() << "Deleted." << std::endl; } <p> <s> statement expression
%destructor { l->dbg() << "Destructor called > "; l->dbg()<<"Delete at " << @$ << ": " << $$ << "... "; free($$); l->dbg() << "Deleted." << std::endl; } <c>

%{
extern int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type* yylloc);
%}

%start script_file
%%

statement	: IF expression statement			{ $$ = new If_Statement($2, $3, l); }
		| IF expression statement ELSE statement	{ $$ = new If_Statement($2, $3, $5, l); }
		| '{' statement_list '}'			{ $$ = $2; }
		| '{' error '}'					{ error(@2,"Syntax error: code block discarded"); $$ = new Statement(l); }
		| VARIABLE '=' expression ';'			{ $$ = new Assignment($1, $3, e, l); free($1); }
		| FUNCTION '(' parameter_list ')' ';'		{ $$ = new Function_Call($1, $3, e, l); }
		| FUNCTION '(' ')' ';'				{ $$ = new Function_Call($1, e, l); }
		| FUNCTION ';'					{ error(@1, "Function missing '()'. Assuming empty parameter list.");
								  $$ = new Function_Call($1, e, l); }
		| error ';'					{ error(@1,"Syntax error: statement discarded"); $$ = new Statement(l); }
		;

statement_list	: statement statement_list			{ $1->link($2); $$ = $1; }
		| statement					{ l->dbg()<<"Statement is "<< $1->describe() << std::endl; $$ = $1;}
		;

expression	: BOOLEAN					{ $$ = new Boolean($1, l); }
		| NUMBER					{ $$ = new Number($1, l); }
		| STRING					{ $$ = new Text(std::string($1), l); free($1); }
		| VARIABLE					{ $$ = new Variable($1, e, l); free($1); }
		| NOT expression				{ $$ = new Not($2, l); }
		| MINUS expression				{ l->dbg()<<"Negative of "<<$2->describe()<<std::endl;$$ = new Negative($2, l); }
		| expression PLUS expression			{ $$ = new Plus($1, $3, l); }
		| expression MINUS expression			{ $$ = new Minus($1, $3, l); }
		| expression MULTIPLY expression		{ $$ = new Multiply($1, $3, l); }
		| expression DIVIDE expression			{ $$ = new Divide($1, $3, l); }
		| expression EQUAL expression			{ $$ = new Equal($1, $3, l); }
		| expression NOT_EQUAL expression		{ $$ = new Not_Equal($1, $3, l); }
		| expression GREATER expression			{ $$ = new Greater($1, $3, l); }
		| expression LESS expression			{ $$ = new Less($1, $3, l); }
		| expression GREATER_EQUAL expression		{ $$ = new Greater_Equal($1, $3, l); }
		| expression LESS_EQUAL expression		{ $$ = new Less_Equal($1, $3, l); }
		| expression OR expression			{ $$ = new Or($1, $3, l); }
		| expression AND expression			{ $$ = new And($1, $3, l); }
		| '(' expression ')'				{ $$ = $2; }
		| '(' error ')'					{ error(@2,"Syntax error: expression discarded"); $$ = new Boolean(false, l); }
		;

parameter	: expression					{ $$ = new Parameter($1, l); }
		;

parameter_list	: parameter
		| parameter ',' parameter_list			{ $$ = $1; $1->link($3); }
		;

script_file	: statement_list				{ l->dbg()<<"End of file"<<std::endl; $1->set_output(); } 
	    	| error						{ error(@1,"Could not parse file"); }
		;

%%

Statement* parse_file(char* file_name, Logger* _l, Environment* _e){
	l=_l;
	e=_e;
	FILE *file=fopen(file_name, "r");
	if(!file) {
		l->err() << "Can't open " << file_name << std::endl;
		return NULL;
	}
	yyin=file;
	yy::parser parser;
#ifdef DEBUG_BUILD
	parser.set_debug_level(1);
#endif
	Statement::reset();
	parser.parse();
	l->dbg()<<"Finished parse."<<std::endl;
	return Statement::get_output();
}

namespace yy {
	void parser::error(const location& loc, const std::string& s){ 
		l->err() << "Error at " << loc << ": " << s << std::endl;
	}
}


