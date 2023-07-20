#ifndef LISP_RUNTIME_ERROR_INCLUDED
#define LISP_RUNTIME_ERROR_INCLUDED

#include <stdexcept>

class LispRunTimeError : public std::runtime_error {
private:
public:
    LispRunTimeError(const std::string& msg): std::runtime_error(msg) {
    }
};

#endif
