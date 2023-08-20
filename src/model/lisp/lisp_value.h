#ifndef LISP_VALUE_INCLUDED
#define LISP_VALUE_INCLUDED

#include <any>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "../not_implemented_error.h"
#include "lisp_function.h"

enum LispValueType {
  LVT_NONE = 0,
  LVT_STRING = 1,
  LVT_DOUBLE = 2,
  LVT_FUNCTION = 3,
  LVT_LIST = 4,
  LVT_IDENTIFIER = 5,
  LVT_BOOL = 6,
  LVT_INTEGER = 7,
};

enum LispBool {
  LISP_BOOL_FALSE = 0,
  LISP_BOOL_TRUE = 1,
};

class LispValue {
private:
  LispValueType _type;
  std::any _content;

public:
  explicit LispValue(LispValueType type) { _type = type; }

  LispValueType type() const { return _type; }

  void set_content(const std::any &content) { _content = content; }

  bool is_none() const { return _type == LVT_NONE; }
  bool is_string() const { return _type == LVT_STRING; }
  bool is_number() const { return _type == LVT_DOUBLE || _type == LVT_INTEGER; }
  bool is_double() const { return _type == LVT_DOUBLE; }
  bool is_integer() const { return _type == LVT_INTEGER; }
  bool is_function() const { return _type == LVT_FUNCTION; }
  bool is_list() const { return _type == LVT_LIST; }
  bool is_identifier() const { return _type == LVT_IDENTIFIER; }
  bool is_boolean() const { return _type == LVT_BOOL; }

  double explicit_double_value() const {
    return std::any_cast<double>(_content);
  }

  int64_t explicit_integer_value() const {
    return std::any_cast<int64_t>(_content);
  }

  std::string string() const { return std::any_cast<std::string>(_content); }

  double to_double() const {
    if (is_integer()) {
      return static_cast<double>(explicit_integer_value());
    }

    return explicit_double_value();
  }

  double to_integer() const {
    if (is_double()) {
      return static_cast<int64_t>(explicit_double_value());
    }

    return explicit_integer_value();
  }

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

    return to_double() == other;
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
      return to_integer() != 0;
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
    if (_type != other._type) {
      return false;
    }

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
    case LVT_DOUBLE:
      return to_double() == other.to_double();
    case LVT_INTEGER:
      return to_integer() == other.to_integer();
    default:
      throw std::runtime_error(
          "Equality check is not implemented for this type");
    }
  }

  bool operator==(const LispValue &other) const {
    return content_equals(other);
  }
};

#endif
