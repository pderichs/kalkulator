#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "../lisp/lisp_function.h"
#include "../lisp/value_converter.h"

#include "location.h"

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

private:
  std::string _formula_content;

  LispValuePtr _lisp_value;

  // Cell knows about its position
  const Location _location;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
