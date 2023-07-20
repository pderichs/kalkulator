#include "lisp_runner.h"
#include "lisp_value.h"

LispRunner::LispRunner(const LispTokens &tokens) { _tokens = tokens; }

LispValue LispRunner::run() const {
  LispValue result;

  int bracket_level = 0;
  bool awaiting_identifier = false;

  for (auto& token: _tokens) {
    if (token.is_space()) {
      continue;
    }

    if (awaiting_identifier && !token.is_identifier()) {
      throw LispRunTimeError("Expected identifier");
    }

    if (token.is_open_bracket()) {
      bracket_level++;
      awaiting_identifier = true;
    }
  }

  return result;
}
