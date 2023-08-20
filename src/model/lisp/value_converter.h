#ifndef VALUE_CONVERTER_INCLUDED
#define VALUE_CONVERTER_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_function.h"

class ValueConverter {
private:
  static LispExecutionContext *execution_context;

public:
  static void set_execution_context(LispExecutionContext *context);

  static LispValuePtr to_lisp_value(const std::string &s);
  static std::string to_string(const LispValuePtr &value,
                               const std::any &context_param);
};

#endif
