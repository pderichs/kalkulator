#include "lisp_parser.h"
#include "lisp_parser_error.h"
#include "lisp_tokens.h"
#include <cctype>
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
    } else if (std::isdigit(current_char()) || c == '-') {
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

  return result;
}

LispToken LispParser::create_number_token(double number) {
  return LispToken{NUMBER, number};
}

LispToken LispParser::create_string_token(const std::string &s) {
  return LispToken{STRING, s};
}

LispToken LispParser::create_identifier_token(const std::string &s) {
  return LispToken{IDENTIFIER, s};
}

LispToken LispParser::create_open_bracket_token() {
  return LispToken{OPEN_BRACKET};
}

LispToken LispParser::create_close_bracket_token() {
  return LispToken{CLOSE_BRACKET};
}

LispToken LispParser::create_space_token() { return LispToken{SPACE}; }

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

  double n = 0.0;

  try {
    n = std::stod(s);
  } catch (std::invalid_argument &iarg) {
    throw LispParserError("Unable to parse number (invalid argument)", s);
  } catch (std::out_of_range &oor) {
    throw LispParserError("Unable to parse number (out of range)", s);
  }

  return create_number_token(n);
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

  while (walk()) {
    char c = current_char();

    if (std::isspace(c) || c == ')') {
      _pos--;
      break;
    } else {
      s += c;
    }
  }

  return create_identifier_token(s);
}
