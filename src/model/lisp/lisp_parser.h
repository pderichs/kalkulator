#ifndef LISP_PARSER_INCLUDED
#define LISP_PARSER_INCLUDED

#include "lisp_tokens.h"
#include "lisp_value.h"
#include <cstddef>
#include <string>

class LispParser {
public:
  LispParser(const std::string &lisp);
  ~LispParser() = default;

  LispTokens parse();

private:
  LispToken create_double_token(LispValue::DoubleType d);
  LispToken create_integer_token(LispValue::IntegerType d);
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

private:
  size_t _pos;
  std::string _lisp;
};

#endif
