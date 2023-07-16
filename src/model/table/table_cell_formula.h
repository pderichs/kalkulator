#ifndef TABLE_CELL_FORMULA_INCLUDED
#define TABLE_CELL_FORMULA_INCLUDED

#include <memory>
#include <string>

class TableCellFormula {
public:
  static bool is_formula(const std::string &content);

  std::string raw_content() { return _content; }

private:
  std::string _content;
};

typedef std::shared_ptr<TableCellFormula> TableCellFormulaPtr;

#endif
