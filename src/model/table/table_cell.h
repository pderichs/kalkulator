#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <string>
#include <vector>
#include <memory>

#include "table_cell_formula.h"

class TableCell {
private:
  TableCellFormulaPtr _formula;
  std::string _visible_content;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
