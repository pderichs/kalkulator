#ifndef LISP_EXECUTOR_ERROR_INCLUDED
#define LISP_EXECUTOR_ERROR_INCLUDED

#include <stdexcept>

class LispExecutionContextError : public std::runtime_error {
public:
  LispExecutionContextError(const std::string &msg) : std::runtime_error(msg) {}
};

#endif
