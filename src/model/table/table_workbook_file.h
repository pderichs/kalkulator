#ifndef TABLE_WORKBOOK_FILE_INCLUDED
#define TABLE_WORKBOOK_FILE_INCLUDED

#include "sqlite3.h"
#include "table_workbook_document.h"
#include <string>

class TableWorkbookFile {
private:
public:
  TableWorkbookFile();
  ~TableWorkbookFile();

  void open(const std::string &filename);
  void close();

  void read(TableWorkbookDocument &workbook);
  void write(const TableWorkbookDocument &workbook);

private:
  void create_tables();

private:
  std::string _filename;
  sqlite3 *_db;
  bool _needs_setup;
};

#endif
