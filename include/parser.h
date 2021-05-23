#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "environment.h"

class Expression;
class Statement;
class Parameter;

static const int FUNC_ECHO  = 0;
static const int FUNC_NEW   = 1;
static const int FUNC_SAVE  = 2;
static const int FUNC_RECT  = 3;
static const int FUNC_TEXT  = 4;
static const int FUNC_IMAGE = 5;
static const int FUNC_LINE  = 6;

extern FILE* yyin;
Statement*   parse_file (char* file_name, Logger* l, Environment* e);

#endif
