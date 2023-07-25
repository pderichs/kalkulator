#ifndef LISP_SYNTAX_CHECKER_INCLUDED
#define LISP_SYNTAX_CHECKER_INCLUDED

#include "lisp_tokens.h"
class LispSyntaxChecker {
private:
  LispTokens _tokens;

public:
  LispSyntaxChecker(const LispTokens& tokens);
  void check() const;

private:
  void check_matching_brackets() const;
};

#endif
