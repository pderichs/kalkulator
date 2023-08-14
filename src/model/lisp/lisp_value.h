#ifndef LISP_VALUE_INCLUDED
#define LISP_VALUE_INCLUDED

#include <any>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../not_implemented_error.h"
#include "lisp_function.h"

enum LispValueType {
  LVT_NONE = 0,
  LVT_STRING = 1,
  LVT_NUMBER = 2,
  LVT_FUNCTION = 3,
  LVT_LIST = 4,
  LVT_IDENTIFIER = 5,
  LVT_BOOL = 6,
};

enum LispBool {
  LISP_BOOL_FALSE = 0,
  LISP_BOOL_TRUE = 1,
};

class LispValue {
private:
  LispValueType _type;
  std::any _content;         // can be one of string, double, LispFunction
  LispValuePtr _func_result; // cached result of function execution

public:
  explicit LispValue() { _type = LVT_NONE; }

  explicit LispValue(const std::string &s) : LispValue(s, false) {}

  explicit LispValue(const std::string &s, bool identifier) {
    if (!identifier) {
      _type = LVT_STRING;
    } else {
      _type = LVT_IDENTIFIER;
    }

    _content = s;
  }

  explicit LispValue(LispBool b) {
    _type = LVT_BOOL;
    _content = b;
  }

  explicit LispValue(double number) {
    _type = LVT_NUMBER;
    _content = number;
  }

  explicit LispValue(const LispFunction &function) {
    _type = LVT_FUNCTION;
    _content = function;
  }

  explicit LispValue(const LispValuePtrVector &list) {
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
  bool is_boolean() const { return _type == LVT_BOOL; }

  std::string string() const { return std::any_cast<std::string>(_content); }
  double number() const { return std::any_cast<double>(_content); }
  bool boolean() const {
    return std::any_cast<LispBool>(_content) == LISP_BOOL_TRUE;
  }
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

  bool operator==(bool &other) const {
    if (!is_boolean()) {
      return false;
    }

    return boolean() == other;
  }

  bool is_truthy() const {
    if (is_none()) {
      return false;
    }

    if (is_boolean()) {
      return boolean() == LISP_BOOL_TRUE;
    }

    if (is_number()) {
      return number() != 0.0;
    }

    return true;
  }

  bool lists_equals(const LispValue &list) const {
    const auto &my_list = this->list();
    const auto &other_list = list.list();

    if (my_list.size() != other_list.size()) {
      return false;
    }

    size_t n = 0;
    for (const auto &item : my_list) {
      const auto &other_item = other_list[n];
      if (!item->content_equals(*other_item)) {
        return false;
      }

      n++;
    }

    return true;
  }

  bool content_equals(const LispValue &other) const {
    if (_type == other._type) {
      switch (_type) {
      case LVT_NONE:
        return this->is_none() && other.is_none();
      case LVT_BOOL:
        return boolean() == other.boolean();
      case LVT_IDENTIFIER:
      case LVT_STRING:
        return string() == other.string();
      case LVT_LIST:
        return lists_equals(other);
      case LVT_NUMBER:
        return number() == other.number();
      default:
        throw std::runtime_error(
            "Equality check is not implemented for this type");
      }
    }

    return false;
  }
};

#endif
