#include "lisp_parser.h"
#include "lisp_tokens.h"

LispParser::LispParser(const std::string &lisp) { _lisp = lisp; }

LispTokens LispParser::parse() {
  LispTokens result;

  std::string identifier;
  std::string number;

  for (auto c : _lisp) {
    switch (c) {
    case '(':
      result.push_back(create_open_bracket_token());
      break;

    case ')':
      result.push_back(create_close_bracket_token());
      break;

    case ' ':
    case '\t':
      result.push_back(create_space_token());
      break;
    case '0' ... '9':
      result.push_back(create_number_token(c));
      break;
    case '.':
      result.push_back(create_dot_token());
      break;
    case '-':
      result.push_back(create_dash_token());
      break;
    }
  }

  return result;
}

LispToken LispParser::create_dash_token() { return LispToken{DASH}; }

LispToken LispParser::create_dot_token() { return LispToken{DOT}; }

LispToken LispParser::create_number_token(char c) { return LispToken{NUMBER, c}; }

LispToken LispParser::create_char_token(char c) { return LispToken{CHAR, c}; }

LispToken LispParser::create_open_bracket_token() { return LispToken{OPEN_BRACKET}; }

LispToken LispParser::create_close_bracket_token() { return LispToken{CLOSE_BRACKET}; }

LispToken LispParser::create_space_token() { return LispToken{SPACE}; }
