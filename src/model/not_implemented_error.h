#ifndef NOT_IMPLEMENTED_ERROR_INCLUDED
#define NOT_IMPLEMENTED_ERROR_INCLUDED

#include <stdexcept>

class NotImplementedError : public std::runtime_error {
public:
  NotImplementedError(const std::string &msg) : std::runtime_error(msg) {}
};

#endif
