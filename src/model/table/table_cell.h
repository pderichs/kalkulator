#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "../lisp/value_converter.h"

#include "location.h"
#include "table_cell_format.h"

class TableCell {
public:
  TableCell(int row, int col) : _location(col, row) {}

  void update_content(const std::string &content);
  std::string visible_content() const;
  std::string get_formula_content() const;
  bool is_formula() const;
  LispValuePtr lisp_value() const { return _lisp_value; }
  bool has_content() const;
  void clear();
  long row() const { return _location.y(); }
  long col() const { return _location.x(); }
  Location location() const { return _location; }

  bool has_format() const { return static_cast<bool>(_format); }
  TableCellFormat format() const { return *_format; }
  void set_format(const TableCellFormat &format) { _format = format; }

private:
  std::string _formula_content;

  LispValuePtr _lisp_value;

  // Cell knows about its position
  const Location _location;

  std::optional<TableCellFormat> _format;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
