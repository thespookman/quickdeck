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

Value::Value (char* _v) {
    v.s  = _v;
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

char* Value::s () {
    if (type == Value::STRING) return v.s;
    if (type == Value::DOUBLE) return strdup (std::to_string (v.d).c_str ());
    if (v.b) return strdup ("TRUE");
    return strdup ("FALSE");
}

std::ostream& operator<< (std::ostream& os, Value v) { return os << v.s (); }

Value Value::operator! () { return Value (!b ()); }

Value Value::operator+ (Value right) {
    if (type == Value::STRING) return Value (strcat (s (), right.s ()));
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
    case Value::STRING: return Value (strcmp (s (), right.s ()) == 0);
    }
    return false;
}

Value Value::operator!= (Value right) {
    if (type != right.type) return Value (true);
    switch (type) {
    case Value::DOUBLE: return Value (d () != right.d ());
    case Value::BOOL: return Value (b () != right.b ());
    case Value::STRING: return Value (strcmp (s (), right.s ()) != 0);
    }
    return false;
}
