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

#include "value_converter.h"
#include "lisp_execution_context.h"
#include "lisp_parser.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_parser.h"
#include "tools.h"
#include "value_conversion_error.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

LispExecutionContext *ValueConverter::execution_context = nullptr;

LispValuePtr ValueConverter::to_lisp_value(const std::string &s) {
  std::string input = pdtools::trim(s);

  if (input.empty()) {
    // Create none value
    return LispValueFactory::new_none();
  }

  if (input[0] == '=') {
    // Formula

    LispValuePtr result;

    // strip equal sign
    std::string formula = input.substr(1);

    LispParser parser(formula);
    LispValueParser value_parser(parser.parse());
    auto opt_value = value_parser.next();
    if (!opt_value) {
      std::stringstream ss;
      ss << "Unable to parse function: " << formula;
      throw ValueConversionError(ss.str());
    }

    result = std::make_shared<LispValue>(*opt_value);

    if (!result->is_function()) {
      throw ValueConversionError("Expression is not a function");
    }

    return result;
  }

  // Check for integer
  auto opt_int =
      pdtools::convert_string_to_number<LispValue::IntegerType>(input);
  if (opt_int) {
    return LispValueFactory::new_integer(*opt_int);
  }

  // Check for double
  auto opt_double =
      pdtools::convert_string_to_number<LispValue::DoubleType>(input);
  if (opt_double) {
    return LispValueFactory::new_double(*opt_double);
  }

  // Must be a string
  return LispValueFactory::new_string(s);
}

std::string ValueConverter::to_string(const LispValuePtr &value,
                                      const std::any &context_param,
                                      UpdateIdType update_id) {
  if (!execution_context) {
    throw std::runtime_error("to_string: Execution context is NULL");
  }

  if (!value || value->is_none()) {
    return "";
  }

  if (value->is_string()) {
    return value->string();
  } else if (value->is_double()) {
    std::stringstream ss;
    ss << std::setprecision(
        std::numeric_limits<LispValue::DoubleType>::max_digits10 - 1)
       << value->to_double();
    return ss.str();
  } else if (value->is_integer()) {
    std::stringstream ss;
    ss << value->to_integer();
    return ss.str();
  } else if (value->is_function()) {
    // Execute function
    try {
      LispValuePtr result = execute_function(value, context_param, update_id);
      return ValueConverter::to_string(result, context_param, update_id);
    } catch (const std::runtime_error &) {
      return "#EXECUTIONERR";
    }
  } else if (value->is_list()) {
    return "#LIST";
  } else if (value->is_boolean()) {
    if (value->boolean()) {
      return "TRUE";
    } else {
      return "FALSE";
    }
  } else if (value->is_error()) {
    return value->string();
  }

  std::stringstream ss;
  ss << "to_string: Unable to convert lisp value of type "
     << static_cast<int>(value->type());
  throw ValueConversionError(ss.str());
}

void ValueConverter::set_execution_context(LispExecutionContext *context) {
  if (!context) {
    throw std::runtime_error("Execution context is NULL");
  }

  ValueConverter::execution_context = context;
}

LispValuePtr ValueConverter::execute_function(const LispValuePtr &function,
                                              const std::any &context_param,
                                              UpdateIdType update_id) {
  return execution_context->execute(function, context_param, update_id);
}

