#ifndef LISP_CLOSURE_DEFINITION_INCLUDED
#define LISP_CLOSURE_DEFINITION_INCLUDED

#include <map>
#include <string>

#include "tools.h"
#include "lisp_value_ptr.h"

//typedef std::map<std::string, LispValuePtr> LispFunctionParameterDefinitions;

// Used for function definitions and closure definitions.
// If used for closure definitions, name is empty.
struct LispFunctionDefinition {
  std::string name;

  StringVector parameter_definitions;
  LispValuePtrVector body;
};

#endif
