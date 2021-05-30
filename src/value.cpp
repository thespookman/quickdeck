#include "value.h"

#include <cstring>

Value::Value (double _v) {
    v.d  = _v;
    type = DOUBLE;
}

Value::Value (int _v) {
    v.d  = _v;
    type = DOUBLE;
}

Value::Value (bool _v) {
    v.b  = _v;
    type = BOOL;
}

Value::Value (std::string _v) {
    v.s  = strdup (_v.c_str ());
    type = STRING;
}

double Value::d () {
    if (type == Value::DOUBLE) return v.d;
    if (type == Value::BOOL && v.b) return 1;
    return 0;
}

bool Value::b () {
    if (type == Value::BOOL) return v.b;
    if (type == Value::DOUBLE && v.d != 0) return true;
    return false;
}

std::string Value::s () {
    if (type == Value::STRING) return std::string (v.s);
    if (type == Value::DOUBLE) return std::to_string (v.d);
    if (v.b) return "TRUE";
    return "FALSE";
}

std::ostream& operator<< (std::ostream& os, Value v) { return os << v.s (); }

Value Value::operator! () { return Value (!b ()); }

Value Value::operator+ (Value right) {
    if (type == Value::STRING) return Value (s () + right.s ());
    return Value (d () + right.d ());
}

Value Value::operator- (Value right) { return Value (d () - right.d ()); }

Value Value::operator* (Value right) { return Value (d () * right.d ()); }

Value Value::operator/ (Value right) { return Value (d () / right.d ()); }

Value Value::operator> (Value right) { return Value (d () > right.d ()); }

Value Value::operator< (Value right) { return Value (d () < right.d ()); }

Value Value::operator>= (Value right) { return Value (d () >= right.d ()); }

Value Value::operator<= (Value right) { return Value (d () <= right.d ()); }

Value Value::operator|| (Value right) { return Value (b () || right.b ()); }

Value Value::operator&& (Value right) { return Value (b () && right.b ()); }

Value Value::operator== (Value right) {
    if (type != right.type) return Value (false);
    switch (type) {
    case Value::DOUBLE: return Value (d () == right.d ());
    case Value::BOOL: return Value (b () == right.b ());
    case Value::STRING: return Value (s ().compare (right.s ()));
    }
    return false;
}

Value Value::operator!= (Value right) {
    if (type != right.type) return Value (true);
    switch (type) {
    case Value::DOUBLE: return Value (d () != right.d ());
    case Value::BOOL: return Value (b () != right.b ());
    case Value::STRING: return Value (!s ().compare (right.s ()));
    }
    return false;
}
