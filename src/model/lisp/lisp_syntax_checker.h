#ifndef LISP_SYNTAX_CHECKER_INCLUDED
#define LISP_SYNTAX_CHECKER_INCLUDED

#include "lisp_tokens.h"

class LispSyntaxChecker {
public:
  LispSyntaxChecker(const LispTokens &tokens);
  void check() const;

private:
  void check_matching_brackets() const;
  void check_valid_functions() const;

private:
  LispTokens _tokens;
};

#endif
