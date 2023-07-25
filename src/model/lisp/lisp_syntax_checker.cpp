#include "lisp_syntax_checker.h"
#include "lisp_parser_error.h"

LispSyntaxChecker::LispSyntaxChecker(const LispTokens &tokens) {
  _tokens = tokens;
}

void LispSyntaxChecker::check() const {
  check_matching_brackets();
}

void LispSyntaxChecker::check_matching_brackets() const {
  int open_brackets = 0;
  int closed_brackets = 0;

  bool expecting_open_bracket = true;

  for (const auto &token : _tokens) {
    if (token.is_open_bracket()) {
      expecting_open_bracket = false;
      open_brackets++;
    } else if (token.is_closed_bracket()) {
      if (expecting_open_bracket) {
        throw LispParserError("Unexpected closing bracket");
      }

      closed_brackets++;

      if (closed_brackets > open_brackets) {
        throw LispParserError("Unexpected closing bracket");
      }
    }
  }

  if (open_brackets != closed_brackets) {
    throw LispParserError("Brackets count mismatch");
  }
}
