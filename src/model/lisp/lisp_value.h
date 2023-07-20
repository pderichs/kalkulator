#ifndef LISP_VALUE_INCLUDED
#define LISP_VALUE_INCLUDED

#include <any>
#include <string>

enum LispValueType { LVT_NONE, LVT_STRING, LVT_NUMBER };

class LispValue {
private:
  LispValueType _type;
  std::any _content;

public:
  LispValue() {
    _type = LVT_NONE;
  }

  LispValue(double number) {
    _type = LVT_NUMBER;
    _content = number;
  }

  LispValue(std::string s) {
    _type = LVT_STRING;
    _content = s;
  }

  bool is_none() const { return _type == LVT_NONE; }
  bool is_string() const { return _type == LVT_STRING; }
  bool is_number() const { return _type == LVT_NUMBER; }
};

#endif
