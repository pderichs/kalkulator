#include "value_converter.h"
#include "lisp_function.h"
#include "lisp_parser.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include "lisp_value_parser.h"
#include "tools.h"
#include "value_conversion_error.h"
#include <memory>
#include <regex>
#include <sstream>
#include <string>

LispValuePtr ValueConverter::to_lisp_value(const std::string &s) {
  LispValuePtr result;

  std::string input = pdtools::trim(s);

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

    std::regex exp_number("^(\\-?\\d+\\.?\\d+)$");
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

std::string ValueConverter::to_string(const LispValuePtr &value) {
  if (value->is_string()) {
    return value->string();
  } else if (value->is_number()) {
    std::stringstream ss;
    ss << value->number();
    return ss.str();
  } else {
    throw ValueConversionError("Unable to convert LispValue");
  }
}
