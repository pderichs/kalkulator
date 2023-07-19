#ifndef LISP_PARSER_INCLUDED
#define LISP_PARSER_INCLUDED

#include "lisp_tokens.h"
#include <string>

class LispParser {
private:
  std::string _lisp;

public:
  LispParser(const std::string &lisp);
  ~LispParser() = default;

  LispTokens parse();

private:
  LispToken create_dash_token();
  LispToken create_dot_token();
  LispToken create_number_token(double d);
  LispToken create_string_token(const std::string &s);
  LispToken create_open_bracket_token();
  LispToken create_close_bracket_token();
  LispToken create_space_token();
};

#endif
