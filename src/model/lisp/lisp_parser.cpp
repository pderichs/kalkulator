#include "lisp_parser.h"
#include "lisp_parser_error.h"
#include "lisp_syntax_checker.h"
#include "lisp_tokens.h"
#include "lisp_value.h"
#include "tools.h"
#include <cctype>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

LispParser::LispParser(const std::string &lisp) { _lisp = lisp; }

LispTokens LispParser::parse() {
  LispTokens result;

  start_parsing();

  do {
    char c = current_char();

    if (c == '"') {
      result.push_back(read_string());
    } else if (c == '-') {
      bool is_number;

      // Looking forward to check whether we have
      // a negative number or an identifier here.
      _pos++;
      is_number = std::isdigit(current_char());
      _pos--;

      if (is_number) {
        result.push_back(read_number());
      } else {
        result.push_back(read_identifier());
      }
    } else if (std::isdigit(current_char())) {
      result.push_back(read_number());
    } else if (std::isspace(current_char())) {
      result.push_back(create_space_token());
    } else if (c == '(') {
      result.push_back(create_open_bracket_token());
    } else if (c == ')') {
      result.push_back(create_close_bracket_token());
    } else if (std::isprint(c)) {
      result.push_back(read_identifier());
    }
  } while (walk());

  LispSyntaxChecker checker(result);
  checker.check();

  return result;
}

LispToken LispParser::create_double_token(LispValue::DoubleType number) {
  return LispToken{DOUBLE, number};
}

LispToken LispParser::create_integer_token(LispValue::IntegerType number) {
  return LispToken{INTEGER, number};
}

LispToken LispParser::create_string_token(const std::string &s) {
  return LispToken{STRING, s};
}

LispToken LispParser::create_identifier_token(const std::string &s) {
  return LispToken{IDENTIFIER, s};
}

LispToken LispParser::create_open_bracket_token() {
  return LispToken{OPEN_BRACKET, {}};
}

LispToken LispParser::create_close_bracket_token() {
  return LispToken{CLOSE_BRACKET, {}};
}

LispToken LispParser::create_space_token() { return LispToken{SPACE, {}}; }

void LispParser::start_parsing() { _pos = 0; }

LispToken LispParser::read_string() {
  std::string s;

  bool escaped = false;

  while (walk()) {
    char c = current_char();

    if (c == '\\') {
      escaped = true;
    }

    if (c == '"') {
      if (!escaped) {
        break;
      } else {
        escaped = false;
      }
    }

    s += c;
  }

  return create_string_token(s);
}

LispToken LispParser::read_number() {
  std::string s;
  bool dot = false;

  do {
    char c = current_char();

    if (std::isdigit(c)) {
      s += c;
    } else if (c == '-') {
      if (s.size() > 0) {
        throw LispParserError("Unexpected dash while parsing number", s);
      }

      s += c;
    } else if (c == '.') {
      if (dot) {
        throw LispParserError("Second dot while parsing number", s);
      }

      s += c;
      dot = true;
    } else if (std::isspace(c) || c == ')') {
      _pos--;
      break;
    }
  } while (walk());

  auto opt_int = pdtools::convert_string_to_number<LispValue::IntegerType>(s);
  if (opt_int) {
    return create_integer_token(*opt_int);
  }

  auto opt_double = pdtools::convert_string_to_number<LispValue::DoubleType>(s);
  if (opt_double) {
    return create_double_token(*opt_double);
  }

  std::stringstream ss;
  ss << "Unable to convert string to number: \"";
  ss << s << "\"";
  throw LispParserError(ss.str());
}

bool LispParser::walk() {
  if (_pos >= _lisp.size()) {
    return false;
  }

  _pos++;

  return true;
}

char LispParser::current_char() const { return _lisp[_pos]; }

LispToken LispParser::read_identifier() {
  std::string s;

  do {
    char c = current_char();

    if (std::isspace(c) || c == ')') {
      _pos--;
      break;
    } else { // TODO check std::isprint?
      s += c;
    }
  } while (walk());

  return create_identifier_token(s);
}
