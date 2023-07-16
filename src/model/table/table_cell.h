#ifndef TABLE_CELL_INCLUDED
#define TABLE_CELL_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "table_cell_formula.h"

class TableCell {
public:
  std::string visible_content() const { return _visible_content; }
  void update_content(const std::string& content);

private:
  void update_visible_content(const std::string &content);

private:
  TableCellFormulaPtr _formula;
  std::string _visible_content;
};

typedef std::shared_ptr<TableCell> TableCellPtr;
typedef std::vector<TableCellPtr> TableRow;
typedef std::shared_ptr<TableRow> TableRowPtr;

#endif
