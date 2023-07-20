#ifndef LISP_TOKENS_INCLUDED
#define LISP_TOKENS_INCLUDED

#include <any>
#include <vector>

enum TokenID {
  OPEN_BRACKET,
  CLOSE_BRACKET,
  IDENTIFIER,
  STRING,
  NUMBER,
  SPACE
};

struct LispToken {
  TokenID id;
  std::any content;
};

class LispTokens : public std::vector<LispToken> {
private:
public:
  LispTokens();
  ~LispTokens() = default;
};

#endif
