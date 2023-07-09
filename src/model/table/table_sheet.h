#ifndef TABLE_SHEET_INCLUDED
#define TABLE_SHEET_INCLUDED

#include <vector>

#include "table_cell.h"

typedef std::vector<TableRow> TableRows;

class TableSheet {
private:
  TableRows _rows;
};

#endif
