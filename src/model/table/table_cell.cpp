#include "table_cell.h"

void TableCell::update_visible_content(const std::string& content) {
  _visible_content = content;

  // TODO send notifications
}
