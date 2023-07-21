#ifndef LISP_VALUE_INCLUDED
#define LISP_VALUE_INCLUDED

#include <any>
#include <string>
#include <vector>

enum LispValueType { LVT_NONE, LVT_STRING, LVT_NUMBER, LVT_FUNCTION };

class LispValue {
private:
  LispValueType _type;
  std::any _content; // can be string, double, LispFunction

public:
  LispValue() { _type = LVT_NONE; }

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

typedef std::vector<LispValue> LispValueVector;

#endif
