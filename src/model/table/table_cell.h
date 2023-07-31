#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "../lisp/lisp_function.h"
#include "../lisp/value_converter.h"

class TableCell {
public:
  TableCell(int row, int col) {
    _row = row;
    _col = col;
  }

  void update_content(const std::string &content);

  std::string visible_content() const;
  std::string get_formula_content() const;
  bool is_formula() const;

  LispValuePtr lisp_value() const { return _lisp_value; }

  bool has_content() const;

  int row() const { return _row; }
  int col() const { return _col; }

private:
  std::string _formula_content;

  LispValuePtr _lisp_value;

  int _row;
  int _col;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
