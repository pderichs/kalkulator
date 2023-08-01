#include "table_cell.h"
#include "../lisp/lisp_value.h"

void TableCell::update_content(const std::string &content) {
  _lisp_value = ValueConverter::to_lisp_value(content);

  if (_lisp_value->is_function()) {
    _formula_content = content;
  } else {
    _formula_content = "";
  }
}

std::string TableCell::visible_content() const {
  return ValueConverter::to_string(_lisp_value, _location);
}

std::string TableCell::get_formula_content() const {
  if (is_formula()) {
    return _formula_content;
  }

  return visible_content();
}

bool TableCell::is_formula() const {
  return _lisp_value && _lisp_value->is_function();
}

bool TableCell::has_content() const {
  return _lisp_value && !_lisp_value->is_none();
}
