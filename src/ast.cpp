#include "ast.h"

#include <cstring>
#include <iostream>

Statement* Statement::OUTPUT;

AST::AST () : description {"AST Node"} {}

std::string AST::describe () { return description; }

void Statement::set_output () { OUTPUT = this; }

Statement* Statement::get_output () { return OUTPUT; }

void Statement::reset () { OUTPUT = NULL; }

Statement::Statement (Logger* _l) : next {NULL}, l {_l} { description = ""; }

void Statement::link (Statement* _next) {
    if (_next == NULL) return;
    if (next == NULL) {
        l->dbg () << "Linking" << std::endl;
        l->dbg () << "\tfrom: \t" << describe () << std::endl;
        l->dbg () << "\tto: \t" << _next->describe () << std::endl;
        next = _next;
        description += next->describe ();
        l->dbg () << "Linked" << std::endl;
    } else
        next->link (_next);
}

Statement* Statement::get_next () { return next; }

Value Statement::evaluate () {
    if (next) return next->evaluate ();
    return Value (true, l);
}

Statement::~Statement () {
    l->dbg () << "Deleting statement " << description << std::endl;
    if (next) {
        delete next;
        next = NULL;
    }
}

Expression::Expression (Logger* _l) : l {_l} {
    l->dbg () << "!" << std::endl;
    description = "EXPRESSION ";
    l->dbg () << description << std::endl;
}

Value Expression::evaluate () { return Value (true, l); }

Expression::~Expression () { l->dbg () << "Deleting expression " << description << std::endl; }

Number::Number (double _value, Logger* _l) : Expression (_l) {
    value       = _value;
    description = std::to_string (value) + " ";
    l->dbg () << "NUMBER: " << value << std::endl;
}

Value Number::evaluate () { return Value (value, l); }

Boolean::Boolean (bool _value, Logger* _l) : Expression (_l) {
    value       = _value;
    description = (value ? "TRUE " : "FALSE ");
    l->dbg () << "BOOLEAN: " << value << std::endl;
}

Value Boolean::evaluate () { return Value (value, l); }

Text::Text (std::string _value, Logger* _l) : Expression (_l) {
    value       = _value;
    description = value + " ";
    l->dbg () << "TEXT: " << value << std::endl;
}

Value Text::evaluate () { return Value (value, l); }

void Text::describe () { std::cout << '"' << value << '"' << std::endl; }

Text::~Text () {}

If_Statement::If_Statement (Expression* _condition, Statement* _then, Statement* _else, Logger* _l)
    : Statement (_l) {
    condition   = _condition;
    then_do     = _then;
    else_do     = _else;
    description = "IF ( " + condition->describe () + ") { \n" + then_do->describe () + "} ";
    if (else_do) description += "ELSE { \n" + else_do->describe () + "}";
    description += "\n";
    l->dbg () << "IF" << std::endl;
}

Value If_Statement::evaluate () {
    if (condition->evaluate ().b ())
        then_do->evaluate ();
    else if (else_do)
        else_do->evaluate ();
    if (next) return next->evaluate ();
    return Value (true, l);
}

If_Statement::If_Statement (Expression* _condition, Statement* _then, Logger* _l)
    : If_Statement (_condition, _then, NULL, _l) {}

If_Statement::~If_Statement () {
    if (condition) {
        delete condition;
        condition = NULL;
    }
    if (then_do) {
        delete then_do;
        then_do = NULL;
    }
    if (else_do) {
        delete else_do;
        else_do = NULL;
    }
    l->dbg () << "Deleting IF" << std::endl;
}

Parameter::Parameter (Expression* _expression, Logger* _l)
    : expression {_expression}, next {NULL}, l {_l} {
    description = expression->describe ();
    l->dbg () << "PARAMETER" << std::endl;
}

void Parameter::get (std::vector<Expression*>* param_list) {
    param_list->push_back (expression);
    if (next) next->get (param_list);
}

void Parameter::link (Parameter* _next) { next = _next; }

Parameter::~Parameter () {
    if (expression) {
        delete expression;
        expression = NULL;
    }
    if (next) {
        delete next;
        next = NULL;
    }
    l->dbg () << "Deleting Parameter" << std::endl;
};

Assignment::Assignment (std::string _variable, Expression* _value, Environment* _e, Logger* _l)
    : Statement (_l) {
    variable    = _variable;
    value       = _value;
    e           = _e;
    description = variable + " = " + value->describe () + ";\n";
    l->dbg () << "ASSIGNMENT " << variable << std::endl;
}

Value Assignment::evaluate () {
    e->set_variable (variable, value->evaluate ());
    l->dbg () << "Variable set" << std::endl;
    if (next) return next->evaluate ();
    return Value (true, l);
}

Assignment::~Assignment () {
    if (value) {
        delete value;
        value = NULL;
    }
    l->dbg () << "Deleting assignment: " << variable << std::endl;
}

Variable::Variable (std::string _variable, Environment* _e, Logger* _l) : Expression (_l) {
    variable    = _variable;
    e           = _e;
    description = variable + " ";
    l->dbg () << "VARIABLE: " << variable << std::endl;
}

Value Variable::evaluate () { return e->get_variable (variable); }
