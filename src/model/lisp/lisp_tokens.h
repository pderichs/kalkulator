#ifndef LISP_TOKENS_INCLUDED
#define LISP_TOKENS_INCLUDED

#include <any>
#include <ostream>
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
  LispTokens() = default;
  ~LispTokens() = default;

  void debug_print(std::ostream& oss) const;
};

std::string token_id_to_string(TokenID token_id);
std::string printable_content_of_token(const LispToken& token);

#endif
