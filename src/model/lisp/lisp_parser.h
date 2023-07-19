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
private:
    LispToken create_dash_token();
    LispToken create_dot_token();
    LispToken create_number_token(char c);
    LispToken create_char_token(char c);
    LispToken create_open_bracket_token();
    LispToken create_close_bracket_token();
    LispToken create_space_token();
};

#endif
