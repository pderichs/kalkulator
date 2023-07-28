#ifndef VALUE_CONVERTER_INCLUDED
#define VALUE_CONVERTER_INCLUDED

#include "lisp_function.h"

class ValueConverter {
public:
    static LispValuePtr to_lisp_value(const std::string& s);
    static std::string to_string(const LispValuePtr& value);
    static std::string to_string(const LispValue& value);
};

#endif
