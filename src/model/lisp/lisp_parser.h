#ifndef LISP_PARSER_INCLUDED
#define LISP_PARSER_INCLUDED

#include <string>
#include "lisp_tokens.h"

class LispParser {
private:
    std::string _lisp;

public:
    LispParser(const std::string& lisp);
    ~LispParser() = default;

    LispTokens parse();
};

#endif
