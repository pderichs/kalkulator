#ifndef LISP_FUNCTION_PARSER_INCLUDED
#define LISP_FUNCTION_PARSER_INCLUDED

#include "lisp_function.h"
#include "lisp_tokens.h"
#include <cstddef>
#include <optional>

class LispValueParser {
private:
  LispTokens _tokens;
  size_t _pos;

public:
  LispValueParser(const LispTokens &tokens, size_t start = 0) {
    _pos = start;
    _tokens = tokens;
  }

  std::optional<LispValue> next();

private:
  bool has_next() const { return _pos < _tokens.size(); }
  LispToken current_token() const { return _tokens[_pos]; }
  void skip_spaces();
  LispValue parse_function();
  LispTokens collect_current_function_tokens();
};

#endif