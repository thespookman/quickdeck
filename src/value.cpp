#include "value.h"

#include <cstring>

Value::Value (double _v, Logger* _l) : l {_l} {
    v.d  = _v;
    type = DOUBLE;
    l->dbg () << "New double value " << s () << std::endl;
}

Value::Value (int _v, Logger* _l) : l {_l} {
    v.d  = _v;
    type = DOUBLE;
    l->dbg () << "New double value " << s () << std::endl;
}

Value::Value (bool _v, Logger* _l) : l {_l} {
    v.b  = _v;
    type = BOOL;
    l->dbg () << "New bool value " << s () << std::endl;
}

Value::Value (const char* _v, Logger* _l) : l {_l} {
    v.s  = strdup (_v);
    type = STRING;
    l->dbg () << "New string value " << s () << std::endl;
}

Value::Value (std::string _v, Logger* _l) : l {_l} {
    const char* c = _v.c_str ();
    v.s           = strdup (c);
    type          = STRING;
    l->dbg () << "New string value " << s () << std::endl;
}

Value::Value (const Value& _v) {
    l    = _v.l;
    type = _v.type;
    switch (type) {
    case DOUBLE: v.d = _v.v.d; break;
    case BOOL: v.b = _v.v.b; break;
    case STRING: v.s = strdup (_v.v.s); break;
    }
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
    if (v.b) return std::string ("TRUE");
    return std::string ("FALSE");
}

std::ostream& operator<< (std::ostream& os, Value v) { return os << v.s (); }

Value Value::operator! () { return Value (!b (), l); }

Value Value::operator+ (Value right) {
    if (type == Value::STRING) return Value (s () + right.s (), l);
    return Value (d () + right.d (), l);
}

Value Value::operator- (Value right) { return Value (d () - right.d (), l); }

Value Value::operator* (Value right) { return Value (d () * right.d (), l); }

Value Value::operator/ (Value right) { return Value (d () / right.d (), l); }

Value Value::operator> (Value right) { return Value (d () > right.d (), l); }

Value Value::operator< (Value right) { return Value (d () < right.d (), l); }

Value Value::operator>= (Value right) { return Value (d () >= right.d (), l); }

Value Value::operator<= (Value right) { return Value (d () <= right.d (), l); }

Value Value::operator|| (Value right) { return Value (b () || right.b (), l); }

Value Value::operator&& (Value right) { return Value (b () && right.b (), l); }

Value Value::operator== (Value right) {
    if (type != right.type) return Value (false, l);
    switch (type) {
    case Value::DOUBLE: return Value (d () == right.d (), l);
    case Value::BOOL: return Value (b () == right.b (), l);
    case Value::STRING: return Value (s ().compare (right.s ()) == 0, l);
    }
    return Value (false, l);
}

Value Value::operator!= (Value right) {
    if (type != right.type) return Value (true, l);
    switch (type) {
    case Value::DOUBLE: return Value (d () != right.d (), l);
    case Value::BOOL: return Value (b () != right.b (), l);
    case Value::STRING: return Value (s ().compare (right.s ()) != 0, l);
    }
    return Value (false, l);
}

Value::~Value () {
    l->dbg () << "Deleting value: " << s () << std::endl;
    if (type == STRING) free (v.s);
    l->dbg () << "Value deleted." << std::endl;
}
