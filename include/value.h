#ifndef VALUE_H_
#define VALUE_H_

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

  public:
    Value (double _v);
    Value (int _v);
    Value (bool _v);
    Value (std::string _v);
    double               d ();
    bool                 b ();
    std::string          s ();
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
};
#endif
