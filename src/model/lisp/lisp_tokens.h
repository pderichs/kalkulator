#ifndef LISP_TOKENS_INCLUDED
#define LISP_TOKENS_INCLUDED

#include <vector>

enum TokenID { OPEN_BRACKET, CLOSE_BRACKET, IDENTIFIER, STRING, NUMBER };

class LispTokens: public std::vector<TokenID> {
private:
public:
  LispTokens();
  ~LispTokens() = default;
};

#endif
