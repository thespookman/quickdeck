#ifndef VALUE_H_
#define VALUE_H_

#include "SPKlog.h"

#include <ostream>
#include <string>

union V {
    double d;
    bool   b;
    char*  s;
};

class Value {
  private:
    int                  type;
    static constexpr int DOUBLE = 0;
    static constexpr int BOOL   = 1;
    static constexpr int STRING = 2;
    V                    v;
    Logger*              l;

  public:
    Value (double _v, Logger* _l);
    Value (int _v, Logger* _l);
    Value (bool _v, Logger* _l);
    Value (std::string _v, Logger* _l);
    Value (const char* _v, Logger* _l);
    Value (const Value& _v);
    double               d () const;
    bool                 b () const;
    std::string          s () const;
    friend std::ostream& operator<< (std::ostream& os, Value v);
    Value                operator! ();
    Value                operator+ (Value right);
    Value                operator- (Value right);
    Value                operator* (Value right);
    Value                operator/ (Value right);
    Value                operator== (Value right);
    Value                operator!= (Value right);
    Value                operator> (Value right);
    Value                operator< (Value right);
    Value                operator>= (Value right);
    Value                operator<= (Value right);
    Value                operator|| (Value right);
    Value                operator&& (Value right);
    void                 operator= (Value right);
    ~Value ();
};
#endif
