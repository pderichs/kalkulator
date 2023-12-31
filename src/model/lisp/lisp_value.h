/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LISP_VALUE_INCLUDED
#define LISP_VALUE_INCLUDED

#include "lisp_function_definition.h"
#include <any>
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>

#include "lisp_value_ptr.h"

/**
 * Each lisp value has a type defined by this enum.
 */
enum LispValueType {
  LVT_NONE = 0,
  LVT_STRING = 1,
  LVT_DOUBLE = 2,
  LVT_LIST = 3,
  LVT_IDENTIFIER = 4,
  LVT_BOOL = 5,
  LVT_INTEGER = 6,
  LVT_FUNCTION_DEFINITION = 7, // non native lisp function
  LVT_ERROR = 8,
};

/**
 * A boolean representation.
 */
enum LispBool {
  LISP_BOOL_FALSE = 0,
  LISP_BOOL_TRUE = 1,
};

/**
 * Combines a type with its content. The content type and data is dependent
 * on the type id.
 */
class LispValue {
private:
  LispValueType _type;
  std::any _content;

public:
  typedef double DoubleType;
  typedef int64_t IntegerType;

  explicit LispValue(LispValueType type) : _type(type), _content({}) {}

  LispValueType type() const { return _type; }

  void set_content(const std::any &content) { _content = content; }

  bool is_none() const { return _type == LVT_NONE; }
  bool is_string() const { return _type == LVT_STRING; }
  bool is_number() const { return is_double() || is_integer(); }
  bool is_double() const { return _type == LVT_DOUBLE; }
  bool is_integer() const { return _type == LVT_INTEGER; }
  bool is_list() const { return _type == LVT_LIST; }
  bool is_identifier() const { return _type == LVT_IDENTIFIER; }
  bool is_boolean() const { return _type == LVT_BOOL; }
  bool is_function_definition() const {
    return _type == LVT_FUNCTION_DEFINITION;
  }
  bool is_error() const { return _type == LVT_ERROR; }

  DoubleType explicit_double_value() const {
    return std::any_cast<DoubleType>(_content);
  }

  IntegerType explicit_integer_value() const {
    return std::any_cast<IntegerType>(_content);
  }

  std::string string() const { return std::any_cast<std::string>(_content); }

  DoubleType to_double() const {
    if (is_integer()) {
      return static_cast<DoubleType>(explicit_integer_value());
    }

    return explicit_double_value();
  }

  IntegerType to_integer() const {
    if (is_double()) {
      return static_cast<IntegerType>(explicit_double_value());
    }

    return explicit_integer_value();
  }

  bool boolean() const {
    return std::any_cast<LispBool>(_content) == LISP_BOOL_TRUE;
  }

  LispValuePtrVector list() const {
    return std::any_cast<LispValuePtrVector>(_content);
  }

  LispFunctionDefinition function_definition() const {
    return std::any_cast<LispFunctionDefinition>(_content);
  }

  bool operator==(const DoubleType &other) const {
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

  // A function is a list with an identifier at position 0.
  bool is_function() const {
    if (!is_list()) {
      return false;
    }

    const auto &lst = list();

    if (lst.empty()) {
      return false;
    }

    const auto &first = lst[0];

    return first->is_identifier();
  }

  bool is_truthy() const {
    if (is_none()) {
      return false;
    }

    if (is_boolean()) {
      return boolean();
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
    case LVT_ERROR:
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
