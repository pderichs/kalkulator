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


#ifndef LISP_VALUE_FACTORY_INCLUDED
#define LISP_VALUE_FACTORY_INCLUDED

#include "lisp_function_definition.h"
#include "lisp_value.h"
#include "lisp_value_ptr.h"
#include <memory>

class LispValueFactory {
private:
  static LispValuePtr new_value_ptr(LispValueType type, const std::any& content) {
    auto result = std::make_shared<LispValue>(type);
    result->set_content(content);
    return result;
  }

public:
  static LispValuePtr new_double(LispValue::DoubleType value) {
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

  static LispValuePtr new_error(const std::string& value) {
    return new_value_ptr(LVT_ERROR, value);
  }

  static LispValuePtr new_integer(LispValue::IntegerType value) {
    return new_value_ptr(LVT_INTEGER, value);
  }

  static LispValuePtr new_none() {
    return std::make_shared<LispValue>(LVT_NONE);
  }

  static LispValuePtr
  new_function_definition(const LispFunctionDefinition &func) {
    return new_value_ptr(LVT_FUNCTION_DEFINITION, func);
  }
};

#endif
