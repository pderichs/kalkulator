#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "table_cell_formula.h"
#include "../lisp/lisp_function.h"

class TableCell {
public:
  std::string visible_content() const { return _visible_content; }
  void update_content(const std::string &content);

  std::string get_formula_content() const {
    if (!_formula) {
      return visible_content();
    }

    return _formula->raw_content();
  }

private:
  void update_visible_content(const std::string &content);

private:
  TableCellFormulaPtr _formula;
  std::string _visible_content;

  LispValuePtr _lisp_value;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
