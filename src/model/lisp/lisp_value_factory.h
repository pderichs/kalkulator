#ifndef LISP_VALUE_FACTORY_INCLUDED
#define LISP_VALUE_FACTORY_INCLUDED

#include "lisp_function.h"
#include "lisp_value.h"
#include <memory>

class LispValueFactory {
private:
  static LispValuePtr new_value_ptr(LispValueType type, std::any content) {
    auto result = std::make_shared<LispValue>(type);
    result->set_content(content);
    return result;
  }

public:
  static LispValuePtr new_double(double value) {
    return new_value_ptr(LVT_DOUBLE, value);
  }

  static LispValuePtr new_string(const std::string &value) {
    return new_value_ptr(LVT_STRING, value);
  }

  static LispValuePtr new_identifier(const std::string &value) {
    return new_value_ptr(LVT_IDENTIFIER, value);
  }

  static LispValuePtr new_list(const LispValuePtrVector &list) {
    return new_value_ptr(LVT_LIST, list);
  }

  static LispValuePtr new_bool(LispBool value) {
    return new_value_ptr(LVT_BOOL, value);
  }

  static LispValuePtr new_integer(int64_t value) {
    return new_value_ptr(LVT_INTEGER, value);
  }

  static LispValuePtr new_function(const LispFunction& function) {
    return new_value_ptr(LVT_FUNCTION, function);
  }

  static LispValuePtr new_none() {
    return std::make_shared<LispValue>(LVT_NONE);
  }
};

#endif
