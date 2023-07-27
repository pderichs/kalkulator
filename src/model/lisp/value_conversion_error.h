#ifndef VALUE_CONVERSION_ERROR_INCLUDED
#define VALUE_CONVERSION_ERROR_INCLUDED

#include <stdexcept>

class ValueConversionError: public std::runtime_error {
public:
    ValueConversionError(const std::string& msg) : std::runtime_error(msg) {}
};

#endif
