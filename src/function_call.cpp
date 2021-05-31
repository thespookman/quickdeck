#include "ast.h"
#include "environment.h"
#include "parser.h"

Function_Call::Function_Call (int _function, Parameter* _parameter, Environment* _e, Logger* _l)
    : Statement (_l) {
    function    = _function;
    parameter   = _parameter;
    e           = _e;
    description = "CALL FUNCTION " + std::to_string (function) + " (";
    std::vector<Expression*> p;
    parameter->get (&p);
    for (Expression* e : p) description += e->describe () + ", ";
    description += ");\n";
    l->dbg () << "FUNCTION: " << function << std::endl;
}

Function_Call::Function_Call (int _function, Environment* _e, Logger* _l)
    : Function_Call (_function, NULL, _e, _l) {}

Value Function_Call::evaluate () {
    std::vector<Expression*> p;
    parameter->get (&p);
    switch (function) {
    case FUNC_ECHO:
        for (Expression* e : p) std::cout << e->evaluate ();
        std::cout << std::endl;
        break;
    case FUNC_NEW:
        if (p.size () < 2) {
            l->err () << "NEW function requires 2 arguments. Have " << p.size () << std::endl;
            break;
        }
        e->canvas.create_canvas (p[0]->evaluate ().d (), p[1]->evaluate ().d ());
        break;
    case FUNC_SAVE:
        if (p.size () < 1) {
            l->err () << "SAVE function requires 1 arguments. Have " << p.size () << std::endl;
            break;
        }
        e->canvas.save_card (p[0]->evaluate ().s ());
        break;
    case FUNC_RECT:
        if (p.size () < 7) {
            l->err () << "RECT function requires 7 arguments. Have " << p.size () << std::endl;
            break;
        }
        e->canvas.rectangle (p[0]->evaluate ().b (), p[1]->evaluate ().d (), p[2]->evaluate ().d (),
                             p[3]->evaluate ().d (), p[4]->evaluate ().d (), p[5]->evaluate ().d (),
                             p[6]->evaluate ().s ());
        break;
    case FUNC_TEXT:
        if (p.size () < 6) {
            l->err () << "TEXT function requires 6 arguments. Have " << p.size () << std::endl;
            break;
        }
        e->canvas.text (p[0]->evaluate ().s (), p[1]->evaluate ().s (), p[2]->evaluate ().d (),
                        p[3]->evaluate ().d (), p[4]->evaluate ().d (), p[5]->evaluate ().s ());
        break;
    case FUNC_IMAGE:
        if (p.size () < 5) {
            l->err () << "IMAGE function requires 5 arguments. Have " << p.size () << std::endl;
            break;
        }
        e->canvas.image (p[0]->evaluate ().s (), p[1]->evaluate ().d (), p[2]->evaluate ().d (),
                         p[3]->evaluate ().d (), p[4]->evaluate ().d ());
        break;
    case FUNC_LINE:
        if (p.size () < 6) {
            l->err () << "LINE function requires 6 arguments. Have " << p.size () << std::endl;
            break;
        }
        e->canvas.line (p[0]->evaluate ().d (), p[1]->evaluate ().d (), p[2]->evaluate ().d (),
                        p[3]->evaluate ().d (), p[4]->evaluate ().d (), p[5]->evaluate ().s ());
        break;
    };
    if (next) return next->evaluate ();
    return Value (true, l);
}

Function_Call::~Function_Call () {
    if (parameter) delete parameter;
    l->dbg () << "Deleting function call: " << function << std::endl;
}
