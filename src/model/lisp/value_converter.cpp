#include "value_converter.h"
#include "lisp_execution_context.h"
#include "lisp_function.h"
#include "lisp_parser.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include "lisp_value_parser.h"
#include "tools.h"
#include "value_conversion_error.h"
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

LispExecutionContext *ValueConverter::execution_context = nullptr;

LispValuePtr ValueConverter::to_lisp_value(const std::string &s) {
  LispValuePtr result;

  std::string input = pdtools::trim(s);

  if (input.empty()) {
    // Create none value
    return std::make_shared<LispValue>();
  }

  if (input[0] == '=') {
    // Formula

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
  } else {
    // Check for number
    std::regex exp_number("^-?(0|[1-9]\\d*)(\\.\\d+)?$");
    std::smatch sm;

    if (std::regex_search(input, sm, exp_number)) {
      double d = std::stod(s);
      result = std::make_shared<LispValue>(d);
    } else {
      // Must be a string
      result = std::make_shared<LispValue>(s);
    }
  }

  return result;
}

std::string ValueConverter::to_string(const LispValuePtr &value,
                                      const std::any &context_param) {
  if (!value) {
    return "";
  }

  return ValueConverter::to_string(*value, context_param);
}

std::string ValueConverter::to_string(const LispValue &value,
                                      const std::any &context_param) {
  if (!execution_context) {
    throw std::runtime_error("to_string: Execution context is NULL");
  }

  if (value.is_string()) {
    return value.string();
  } else if (value.is_number()) {
    std::stringstream ss;
    ss << value.number();
    return ss.str();
  } else if (value.is_none()) {
    return "";
  } else if (value.is_function()) {
    // Execute function
    try {
      LispValue result = execution_context->execute(value, context_param);
      return ValueConverter::to_string(result, context_param);
    } catch (const std::runtime_error &) {
      return "#ERR";
    }
  } else if (value.is_list()) {
    return "#LIST";
  } else if (value.is_boolean()) {
    if (value.boolean()) {
      return "TRUE";
    } else {
      return "FALSE";
    }
  } else {
    std::stringstream ss;
    ss << "Unable to convert lisp value of type " << (int)value.type();
    throw ValueConversionError(ss.str());
  }
}

void ValueConverter::set_execution_context(LispExecutionContext *context) {
  if (!context) {
    throw std::runtime_error("Execution context is NULL");
  }

  ValueConverter::execution_context = context;
}
