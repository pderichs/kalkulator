#ifndef LISP_VALUE_INCLUDED
#define LISP_VALUE_INCLUDED

#include <any>
#include <memory>
#include <string>
#include <vector>

#include "../not_implemented_error.h"
#include "lisp_function.h"

enum LispValueType {
  LVT_NONE,
  LVT_STRING,
  LVT_NUMBER,
  LVT_FUNCTION,
  LVT_LIST,
  LVT_IDENTIFIER,
};

class LispValue {
private:
  LispValueType _type;
  std::any _content;         // can be one of string, double, LispFunction
  LispValuePtr _func_result; // cached result of function execution

public:
  LispValue() { _type = LVT_NONE; }

  LispValue(const std::string &s): LispValue(s, false) {
  }

  LispValue(const std::string &s, bool identifier) {
    if (!identifier) {
      _type = LVT_STRING;
    } else {
      _type = LVT_IDENTIFIER;
    }

    _content = s;
  }

  LispValue(double number) {
    _type = LVT_NUMBER;
    _content = number;
  }

  LispValue(const LispFunction &function) {
    _type = LVT_FUNCTION;
    _content = function;
  }

  LispValue(const LispValuePtrVector &list) {
    _type = LVT_LIST;
    _content = list;
  }

  LispValueType type() const { return _type; }

  bool is_none() const { return _type == LVT_NONE; }
  bool is_string() const { return _type == LVT_STRING; }
  bool is_number() const { return _type == LVT_NUMBER; }
  bool is_function() const { return _type == LVT_FUNCTION; }
  bool is_list() const { return _type == LVT_LIST; }
  bool is_identifier() const { return _type == LVT_IDENTIFIER; }

  std::string string() const { return std::any_cast<std::string>(_content); }
  double number() const { return std::any_cast<double>(_content); }
  LispFunction function() const {
    return std::any_cast<LispFunction>(_content);
  }
  LispValuePtrVector list() const {
    return std::any_cast<LispValuePtrVector>(_content);
  }

  bool operator==(const double &other) const {
    if (!is_number()) {
      return false;
    }

    return number() == other;
  }

  bool operator==(const std::string &other) const {
    if (!is_string() && !is_identifier()) {
      return false;
    }

    return string() == other;
  }

  // TODO: Prepare lisp eq functionality
  bool lisp_eq(const LispValue &other) const {
    throw NotImplementedError("eq is not implemented yet");
  }
};

#endif
