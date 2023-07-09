#ifndef TABLE_CELL_FORMULA_INCLUDED
#define TABLE_CELL_FORMULA_INCLUDED

#include <string>
#include <memory>

class TableCellFormula {
private:
  std::string _content;
};

typedef std::shared_ptr<TableCellFormula> TableCellFormulaPtr;

#endif
