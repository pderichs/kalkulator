#include "lisp_parser.h"
#include "lisp_parser_error.h"
#include "lisp_tokens.h"
#include <cctype>

LispParser::LispParser(const std::string &lisp) { _lisp = lisp; }

LispTokens LispParser::parse() {
  LispTokens result;

  start_parsing();

  while (walk()) {
    if (current_char() == '"') {
      result.push_back(read_string());
    } else if (std::isdigit(current_char()) || current_char() == '-') {
      result.push_back(read_number());
    } else if (std::isspace(current_char())) {
      result.push_back(create_space_token());
    } else if (current_char() == '(') {
      result.push_back(create_open_bracket_token());
    } else if (current_char() == ')') {
      result.push_back(create_close_bracket_token());
    } else {
      result.push_back(read_identifier());
    }
  }

  return result;
}

LispToken LispParser::create_number_token(double number) {
  return LispToken{NUMBER, number};
}

LispToken LispParser::create_string_token(const std::string &s) {
  return LispToken{STRING, s};
}

LispToken LispParser::create_open_bracket_token() {
  return LispToken{OPEN_BRACKET};
}

LispToken LispParser::create_close_bracket_token() {
  return LispToken{CLOSE_BRACKET};
}

LispToken LispParser::create_space_token() { return LispToken{SPACE}; }

void LispParser::start_parsing() {
  _pos = 0;
}

LispToken LispParser::read_string() {
  return create_string_token("Hello"); // TODO
}

LispToken LispParser::read_number() {
  return create_number_token(293);
}

bool LispParser::walk() {
  if (_pos >= _lisp.size()) {
    return false;
  }

  _pos++;

  return true;
}

char LispParser::current_char() const {
  return _lisp[_pos];
}

LispToken LispParser::read_identifier() {
  return LispToken{IDENTIFIER};
}
