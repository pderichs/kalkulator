#ifndef TABLE_SHEET_INCLUDED
#define TABLE_SHEET_INCLUDED

#include <memory>
#include <string>
#include <vector>

#include "table_cell.h"

typedef std::vector<TableRowPtr> TableRows;

class TableSheet {
public:
  TableSheet(const std::string &name);
  ~TableSheet();

  std::string name() const { return _name; }

private:
  TableRows _rows;
  std::string _name;
};

typedef std::shared_ptr<TableSheet> TableSheetPtr;
typedef std::vector<TableSheetPtr> TableSheets;

#endif
