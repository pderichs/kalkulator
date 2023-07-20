#ifndef LISP_PARSER_INCLUDED
#define LISP_PARSER_INCLUDED

#include "lisp_tokens.h"
#include <cstddef>
#include <string>

class LispParser {
private:
  std::string _lisp;

public:
  LispParser(const std::string &lisp);
  ~LispParser() = default;

  LispTokens parse();

private:
  size_t _pos;

private:
  LispToken create_number_token(double d);
  LispToken create_string_token(const std::string &s);
  LispToken create_identifier_token(const std::string &s);
  LispToken create_open_bracket_token();
  LispToken create_close_bracket_token();
  LispToken create_space_token();

  void start_parsing();
  LispToken read_string();
  LispToken read_number();
  LispToken read_identifier();
  bool walk();
  char current_char() const;
};

#endif
