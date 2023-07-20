#ifndef LISP_PARSER_ERROR_INCLUDED
#define LISP_PARSER_ERROR_INCLUDED

#include <stdexcept>
#include <string>

class LispParserError : public std::runtime_error {
private:
  std::string _item;

public:
  LispParserError(const std::string &msg) : std::runtime_error(msg) {}
  LispParserError(const std::string &msg, const std::string &item)
      : std::runtime_error(msg) {
    _item = item;
  }

  std::string item() const { return _item; }
};

#endif
