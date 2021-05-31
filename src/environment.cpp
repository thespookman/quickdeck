#include "environment.h"

Environment::Environment (Logger* _l) : l {_l} {}

void Environment::set_variable (std::string identifier, Value value) {
    l->dbg () << "Setting " << identifier << " to " << value << std::endl;
    Varmap::iterator it = variables.find (identifier);
    if (it == variables.end ()) {
        variables.insert ({identifier, value});
        l->dbg () << "New variable created" << std::endl;
    } else
        it->second = value;
}

Value Environment::get_variable (std::string identifier) {
    Varmap::iterator it = variables.find (identifier);
    if (it == variables.end ())
        return Value (false, l);
    else
        return it->second;
}
