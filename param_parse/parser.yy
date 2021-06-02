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
#include <sstream>
#include <stdexcept>
#include <vector>

extern FILE *yyin;

Logger* l;

std::vector<std::string> header_vec;
std::vector<Value*> data_vec;

std::ofstream ofs;
std::ifstream ifs;

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
%destructor { l->dbg() << "Destructor called > "; l->dbg()<<"Delete at " << @$ << ": " << $$ << "... "; free($$); l->dbg() << "Deleted." << std::endl; } <c>

%{
extern int yylex(yy::parser::semantic_type *yylval, yy::parser::location_type* yylloc);
%}

%start data_file
%%

data_file	: header_line data_lines 	{}
	  	;

header_line	: headers ENDL			{}
		;

headers		: VARIABLE			{ header_vec.push_back($1); free($1); }
	   	| headers DELIM VARIABLE 	{ header_vec.push_back($3); free($3); }
		;

data_lines	: data_line			{ process_line(); }
	   	| data_lines ENDL		{}
	   	| data_lines ENDL data_line	{ process_line(); }
		;

data_line	: data				{ data_vec.push_back($1); }
	  	| data_line DELIM data		{ data_vec.push_back($3); }
		;

data		: STRING			{ $$ = new Value($1,l); free($1); }
		| NUMBER	 		{ $$ = new Value($1,l); }
		| BOOLEAN			{ $$ = new Value($1,l); }
		;
%%

void clear_vec(){
	for (Value* v : data_vec) delete v;
	data_vec.clear();
}

void process_line(){
	unsigned int cols=header_vec.size();
	if(cols==0) {
		l->err()<<"No data. Ending."<<std::endl;
		exit(3);
	}
	l->dbg() << "Line: ";
	for(Value* v : data_vec) l->dbg() << *v << ", ";
	l->dbg() << std::endl;
	if(data_vec.size()<cols) {
		l->err()<<"Bad line. Skipping."<<std::endl;
		clear_vec();
		return;
	}
	std::stringstream ss;
	for(unsigned int i=0; i<cols; ++i){
		try {
			ss << header_vec[i] << " = " << *data_vec[i] << "; ";		
		} catch (std::exception& e) {
			l->err() << e.what() << std::endl;
		}
	}
	std::string to_write = ss.str();
	std::string tmp=data_vec[0]->s();
	std::string file_name = tmp.substr(1,tmp.length()-2) + ".qd";
	try {
		ifs.open(file_name);
		std::string old_data;
		std::getline(ifs, old_data);
		ifs.close();
		if(to_write.compare(old_data)==0) {
			l->dbg() << file_name << " is unchanged. Skipping." << std::endl;
			clear_vec();
			return;
		}
	} catch (std::exception& e) {
		l->inf() << "Couldn't open " << file_name << " for reading. Skipping to writing." << std::endl;
	}
	try {
		ofs.open(file_name, std::ofstream::out | std::ofstream::trunc);
		ofs << to_write;
		ofs.close();
	} catch (std::exception& e) {
		l->err() << "Could not open "<< file_name << " for writing: " << std::endl;
		l->err() << e.what() << std::endl;
		l->err() << "Skipping." <<std::endl;
		clear_vec();
		return;
	}
	clear_vec();
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
	delete l;
	return 0;
}

namespace yy {
	void parser::error(const location& loc, const std::string& s){ 
		l->err() << "Error at " << loc << ": " << s << std::endl;
	}
}


