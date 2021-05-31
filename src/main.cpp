#include "ast.h"
#include "environment.h"
#include "parser.h"
#include "script.tab.hh"

#include <iostream>

int main (int argc, char** argv) {
    int log_level = Logger::INFO;
#ifdef DEBUG_BUILD
    log_level = Logger::DEBUG;
#endif
    Logger* l = new Logger (log_level);
    if (argc < 2) {
        l->err () << "Not enough arguments" << std::endl;
        return 1;
    }
    Environment* e    = new Environment (l);
    Statement*   tree = parse_file (argv[1], l, e);
    if (!tree) return 1;
    for (int i = 2; i < argc; ++i) tree->link (parse_file (argv[i], l, e));
    if (!tree) return 1;
    l->dbg () << tree->describe () << std::endl;
    tree->evaluate ();
    delete tree;
    delete l;
    return 0;
}
