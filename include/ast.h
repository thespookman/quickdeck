#ifndef AST_H
#define AST_H
#include "SPKlog.h"
#include "environment.h"
#include "value.h"

#include <string>
#include <vector>

class AST {
  protected:
    std::string description;

  public:
    AST ();
    std::string describe ();
};

class Statement : public AST {
  protected:
    Statement* next;
    Logger*    l;

  public:
    static Statement* OUTPUT;
    static Statement* get_output ();
    static void       reset ();
    Statement (Logger* _l);
    void          link (Statement* _next);
    void          set_output ();
    Statement*    get_next ();
    virtual Value evaluate ();
    virtual ~Statement ();
};

class Expression : public AST {
  protected:
    Logger* l;

  public:
    Expression (Logger* _l);
    virtual Value evaluate ();
    virtual ~Expression ();
};

class Number : public Expression {
  private:
    double value;

  public:
    Number (double _value, Logger* _l);
    void  describe ();
    Value evaluate () override;
};

class Boolean : public Expression {
  private:
    bool value;

  public:
    Boolean (bool _value, Logger* _l);
    void  describe ();
    Value evaluate () override;
};

class Text : public Expression {
  private:
    std::string value;

  public:
    Text (std::string _value, Logger* _l);
    void  describe ();
    Value evaluate () override;
    ~Text ();
};

class If_Statement : public Statement {
  private:
    Expression* condition;
    Statement*  then_do;
    Statement*  else_do;

  public:
    If_Statement (Expression* _condition, Statement* _then, Logger* _l);
    If_Statement (Expression* _condition, Statement* _then, Statement* _else, Logger* _l);
    void  describe ();
    Value evaluate () override;
    ~If_Statement ();
};

class Parameter : public AST {
  private:
    Expression* expression;
    Parameter*  next;
    Logger*     l;

  public:
    Parameter (Expression* _expression, Logger* _l);
    void get (std::vector<Expression*>* param_list);
    void link (Parameter* _next);
    ~Parameter ();
};

class Function_Call : public Statement {
  private:
    int          function;
    Parameter*   parameter;
    Environment* e;

  public:
    Function_Call (int _function, Parameter* _parameter, Environment* _e, Logger* _l);
    Function_Call (int _function, Environment* _e, Logger* _l);
    void  describe ();
    Value evaluate () override;
    ~Function_Call ();
};

class Variable : public Expression {
  private:
    std::string  variable;
    Environment* e;

  public:
    Variable (std::string _variable, Environment* _e, Logger* _l);
    void  describe ();
    Value evaluate () override;
};

class Assignment : public Statement {
  private:
    std::string  variable;
    Expression*  value;
    Environment* e;

  public:
    Assignment (std::string _variable, Expression* _value, Environment* _e, Logger* _l);
    void  describe ();
    Value evaluate () override;
    ~Assignment ();
};
#endif
