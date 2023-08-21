#include "lisp_syntax_checker.h"
#include "lisp_parser_error.h"
#include <sstream>

LispSyntaxChecker::LispSyntaxChecker(const LispTokens &tokens) {
  _tokens = tokens;
}

void LispSyntaxChecker::check() const {
  check_matching_brackets();
  check_valid_functions();
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
        throw LispParserError("Unexpected closing bracket (2)");
      }
    }
  }

  if (open_brackets != closed_brackets) {
    std::stringstream ss;
    ss << "Brackets count mismatch: " << open_brackets << "/"
       << closed_brackets;
    throw LispParserError(ss.str());
  }

  if (open_brackets == 0 || closed_brackets == 0) {
    throw LispParserError("Not a lisp expression");
  }
}

void LispSyntaxChecker::check_valid_functions() const {
  bool next_identifier = false;

  for (const auto &token : _tokens) {
    if (token.is_open_bracket()) {
      next_identifier = true;
    } else if (next_identifier) {
      if (!token.is_identifier()) {
        throw LispParserError("Expected function call identifier");
      }

      next_identifier = false;
    }
  }
}
