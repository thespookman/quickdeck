#include "operator.h"

#include <cstring>
#include <iostream>

Operator::Operator (std::string _op, Logger* _l) : Expression (_l) {
    l->dbg () << "OPERATOR " << op << std::endl;
    op = _op;
}

Value Operator::evaluate () { return Value (true, l); }

One_Operator::One_Operator (Expression* _operand, std::string _op, Logger* _l)
    : Operator (_op, _l) {
    l->dbg () << "Building single operand operator." << std::endl;
    operand     = _operand;
    description = "( " + op + operand->describe () + ") ";
    l->dbg () << "OPERATOR " << description << std::endl;
}

One_Operator::~One_Operator () {
    if (operand) delete operand;
    l->dbg () << "Deleting Operator: " << op << std::endl;
}

Not::Not (Expression* _operand, Logger* _l) : One_Operator (_operand, "!", _l) {}

Value Not::evaluate () {
    if (operand) return !(operand->evaluate ());
    return Value (true, l);
}

Negative::Negative (Expression* _operand, Logger* _l) : One_Operator (_operand, "-", _l) {}

Value Negative::evaluate () {
    if (operand) return Value (0 - operand->evaluate ().d (), l);
    return Value (0, l);
}

Two_Operator::Two_Operator (Expression* _left, Expression* _right, std::string _op, Logger* _l)
    : Operator (_op, _l) {
    left        = _left;
    right       = _right;
    description = "( " + left->describe () + op + " " + right->describe () + ") ";
}

Two_Operator::~Two_Operator () {
    if (left) delete left;
    if (right) delete right;
}

Plus::Plus (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "+", _l) {}

Value Plus::evaluate () {
    if (left && right) return left->evaluate () + right->evaluate ();
    return Value (0, l);
}

Minus::Minus (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "-", _l) {}

Value Minus::evaluate () {
    if (left && right) return left->evaluate () - right->evaluate ();
    return Value (0, l);
}

Multiply::Multiply (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "*", _l) {}

Value Multiply::evaluate () {
    if (left && right) return left->evaluate () * right->evaluate ();
    return Value (0, l);
}

Divide::Divide (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "/", _l) {}

Value Divide::evaluate () {
    if (left && right) return left->evaluate () / right->evaluate ();
    return Value (0, l);
}

Equal::Equal (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "==", _l) {}

Value Equal::evaluate () {
    if (left && right) return left->evaluate () == right->evaluate ();
    return Value (true, l);
}

Not_Equal::Not_Equal (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "!=", _l) {}

Value Not_Equal::evaluate () {
    if (left && right) return left->evaluate () != right->evaluate ();
    return Value (true, l);
}

Greater::Greater (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, ">", _l) {}

Value Greater::evaluate () {
    if (left && right) return left->evaluate () > right->evaluate ();
    return Value (true, l);
}

Less::Less (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "<", _l) {}

Value Less::evaluate () {
    if (left && right) return left->evaluate () < right->evaluate ();
    return Value (true, l);
}

Greater_Equal::Greater_Equal (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, ">=", _l) {}

Value Greater_Equal::evaluate () {
    if (left && right) return left->evaluate () >= right->evaluate ();
    return Value (true, l);
}

Less_Equal::Less_Equal (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "<=", _l) {}

Value Less_Equal::evaluate () {
    if (left && right) return left->evaluate () <= right->evaluate ();
    return Value (true, l);
}

Or::Or (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "||", _l) {}

Value Or::evaluate () {
    if (left && right) return left->evaluate () || right->evaluate ();
    return Value (true, l);
}

And::And (Expression* _left, Expression* _right, Logger* _l)
    : Two_Operator (_left, _right, "&&", _l) {}

Value And::evaluate () {
    if (left && right) return left->evaluate () && right->evaluate ();
    return Value (true, l);
}
