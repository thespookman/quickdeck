#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_
#include "SPKlog.h"
#include "canvas.h"
#include "value.h"

#include <string>
#include <unordered_map>

typedef std::unordered_map<std::string, Value> Varmap;

class Environment {
  private:
    Varmap  variables;
    Logger* l;

  public:
    Environment (Logger* _l);
    Canvas canvas = Canvas (l);
    void   set_variable (std::string identifier, Value value);
    Value  get_variable (std::string identifier);
};
#endif
