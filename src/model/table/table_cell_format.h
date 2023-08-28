#ifndef TABLE_CELL_FORMAT_INCLUDED
#define TABLE_CELL_FORMAT_INCLUDED

#include "table_cell_color.h"
#include <optional>

struct TableCellFormat {
  // Font
  std::optional<size_t> font_size;
  std::optional<std::string> font_name;
  std::optional<bool> bold;
  std::optional<bool> italic;
  std::optional<bool> underlined;

  // Colors
  std::optional<TableCellColor> background_color;
  std::optional<TableCellColor> foreground_color;
};

#endif
