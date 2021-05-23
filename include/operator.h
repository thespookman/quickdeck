#ifndef OPERATOR_H
#define OPERATOR_H
#include "SPKlog.h"
#include "ast.h"
#include "value.h"

#include <string>

class Operator : public Expression {
  protected:
    std::string op;

  public:
    Operator (std::string _op, Logger* _l);
    Value evaluate () override;
};

class One_Operator : public Operator {
  protected:
    Expression* operand;

  public:
    One_Operator (Expression* _operand, std::string _op, Logger* _l);
    void describe ();
    ~One_Operator ();
};

class Not : public One_Operator {
  public:
    Not (Expression* _operand, Logger* _l);
    Value evaluate () override;
};

class Negative : public One_Operator {
  public:
    Negative (Expression* _operand, Logger* _l);
    Value evaluate () override;
};

class Two_Operator : public Operator {
  protected:
    Expression* left;
    Expression* right;

  public:
    Two_Operator (Expression* _left, Expression* _right, std::string _op, Logger* _l);
    void describe ();
    ~Two_Operator ();
};

class Plus : public Two_Operator {
  public:
    Plus (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Minus : public Two_Operator {
  public:
    Minus (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Multiply : public Two_Operator {
  public:
    Multiply (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Divide : public Two_Operator {
  public:
    Divide (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Equal : public Two_Operator {
  public:
    Equal (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Not_Equal : public Two_Operator {
  public:
    Not_Equal (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Greater : public Two_Operator {
  public:
    Greater (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Less : public Two_Operator {
  public:
    Less (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Greater_Equal : public Two_Operator {
  public:
    Greater_Equal (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Less_Equal : public Two_Operator {
  public:
    Less_Equal (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class Or : public Two_Operator {
  public:
    Or (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};

class And : public Two_Operator {
  public:
    And (Expression* _left, Expression* _right, Logger* _l);
    Value evaluate () override;
};
#endif
