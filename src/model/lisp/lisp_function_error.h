#ifndef LISP_FUNCTION_ERROR_INCLUDED
#define LISP_FUNCTION_ERROR_INCLUDED

#include <stdexcept>
class LispFunctionError : public std::runtime_error {
public:
  LispFunctionError(const std::string &msg) : std::runtime_error(msg) {}
};

#endif
