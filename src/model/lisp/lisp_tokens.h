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

  bool is_space() const {
    return id == SPACE;
  }

  bool is_identifier() const {
    return id == IDENTIFIER;
  }

  bool is_string() const {
    return id == STRING;
  }

  bool is_number() const {
    return id == NUMBER;
  }

  bool is_open_bracket() const {
    return id == OPEN_BRACKET;
  }

  bool is_closed_bracket() const {
    return id == CLOSE_BRACKET;
  }
};

class LispTokens : public std::vector<LispToken> {
private:
public:
  LispTokens();
  ~LispTokens() = default;
};

#endif
