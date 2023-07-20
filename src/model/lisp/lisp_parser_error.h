#ifndef LISP_PARSER_ERROR_INCLUDED
#define LISP_PARSER_ERROR_INCLUDED

#include <stdexcept>

class LispParserError : public std::runtime_error {
public:
  LispParserError(const std::string &msg) : std::runtime_error(msg) {}
};

#endif
