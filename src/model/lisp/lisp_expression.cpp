#include "lisp_expression.h"
#include "lisp_tokens.h"

LispExpression::LispExpression(const LispTokens& tokens) {
  _tokens = tokens;
}
