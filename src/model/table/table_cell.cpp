#include "table_cell.h"
#include "table_cell_formula.h"

void TableCell::update_visible_content(const std::string& content) {
  _visible_content = content;
}

void TableCell::update_content(const std::string& content) {
  if (TableCellFormula::is_formula(content)) {
    // TODO Handle formula
  } else {
    update_visible_content(content);
  }
}
