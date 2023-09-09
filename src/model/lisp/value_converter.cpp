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
#include <ios>
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

    if (!result->is_function_or_possible_lambda()) {
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

// TODO Maybe add cell specific formats
std::string ValueConverter::to_string(const LispValuePtr &value,
                                      const std::any &context_param) {
  if (!execution_context) {
    throw std::runtime_error("to_string: Execution context is NULL");
  }

  if (!value) {
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
  } else if (value->is_none()) {
    return "";
  } else if (value->is_function_or_possible_lambda()) {
    // Execute function
    try {
      LispValuePtr result = execution_context->execute(value, context_param);
      return ValueConverter::to_string(result, context_param);
    } catch (const std::runtime_error &) {
      return "#ERR";
    }
  } else if (value->is_list()) {
    return "#LIST";
  } else if (value->is_boolean()) {
    if (value->boolean()) {
      return "TRUE";
    } else {
      return "FALSE";
    }
  } else {
    std::stringstream ss;
    ss << "Unable to convert lisp value of type "
       << static_cast<int>(value->type());
    throw ValueConversionError(ss.str());
  }
}

void ValueConverter::set_execution_context(LispExecutionContext *context) {
  if (!context) {
    throw std::runtime_error("Execution context is NULL");
  }

  ValueConverter::execution_context = context;
}
