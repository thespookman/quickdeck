%language "C++"
%defines
%define parse.trace
%locations

%{
#include "SPKlog.h"
#include "data.h"
#include "value.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <vector>

extern FILE *yyin;

Logger* l;

std::vector<std::string> header_vec;
std::vector<Value*> data_vec;

std::ofstream ofs;

void process_line();

%}

%union {
	char* c;
	double d;
	bool b;
	Value* v;
};

%token <c> STRING VARIABLE
%token <b> BOOLEAN
%token <d> NUMBER
%token DELIM ENDL

%type <v> data

%destructor { l->dbg() << "Destructor called > "; l->dbg()<<"Delete at " << @$ << ": " << $$->s() << "... "; delete $$; l->dbg() << "Deleted." << std::endl; } <v>
%destructor { l->dbg() << "Destructor called > "; l->dbg()<<"Delete at " << @$ << ": " << $$ << "... "; delete $$; l->dbg() << "Deleted." << std::endl; } <c>

%{
extern int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type* yylloc);
%}

%start data_file
%%

data_file	: header_line data_lines 	{}
	  	;

header_line	: headers ENDL			{}
		;

headers		: VARIABLE			{ header_vec.push_back($1); }
	   	| headers DELIM VARIABLE 	{ header_vec.push_back($3); }
		;

data_lines	: data_line			{ process_line(); }
	   	| data_lines ENDL		{}
	   	| data_lines ENDL data_line	{ process_line(); }
		;

data_line	: data				{ data_vec.push_back($1); }
	  	| data_line DELIM data		{ data_vec.push_back($3); }
		;

data		: STRING			{ $$ = new Value($1); }
		| NUMBER	 		{ $$ = new Value($1); }
		| BOOLEAN			{ $$ = new Value($1); }
		;
%%

void process_line(){
	unsigned int cols=header_vec.size();
	if(cols==0) {
		l->err()<<"No data. Ending."<<std::endl;
		exit(3);
	}
	l->dbg() << "Line: ";
	for(Value* v:data_vec) l->dbg() << v << ", ";
	l->dbg() << std::endl;
	if(data_vec.size()<cols) {
		l->err()<<"Bad line. Skipping."<<std::endl;
		data_vec.clear();
		return;
	}
	try {
		ofs.open(data_vec[0]->s());
	} catch (std::exception& e) {
		l->err() << "Could not open "<< data_vec[0] << " for writing: " << std::endl;
		l->err() << e.what() << std::endl;
		l->err() << "Skipping." <<std::endl;
		data_vec.clear();
		return;
	}
	for(unsigned int i=0; i<cols; ++i){
		try {
			ofs << header_vec[i] << " = " << *data_vec[i] << ";" << std::endl;		
		} catch (std::exception& e) {
			l->err() << e.what() << std::endl;
		}
	}
	ofs.close();
	data_vec.clear();
}

int main (int argc, char** argv) {
	int log_level = Logger::INFO;
#ifdef DEBUG_BUILD
    	log_level = Logger::DEBUG;
#endif
	l = new Logger (log_level);
	if (argc < 2) {
		l->err () <<"Not enough arguments." << std::endl;
		return 1;
	}
	FILE *file=fopen(argv[1],"r");
        if(!file) {
                l->err() << "Can't open " << argv[1] << std::endl;
                return 2;
        }
        yyin=file;
        yy::parser parser;
#ifdef DEBUG_BUILD
        parser.set_debug_level(1);
#endif
	parser.parse();
	l->dbg() << "Finished parse." << std::endl;
	return 0;
}

namespace yy {
	void parser::error(const location& loc, const std::string& s){ 
		l->err() << "Error at " << loc << ": " << s << std::endl;
	}
}


