#ifndef TABLE_WORKBOOK_FILE_INCLUDED
#define TABLE_WORKBOOK_FILE_INCLUDED

#include "sqlite3.h"
#include "table_sheet.h"
#include "table_workbook_document.h"
#include "tools.h"
#include <string>

class TableWorkbookFile {
public:
  TableWorkbookFile();
  ~TableWorkbookFile();

  void open(const std::string &filename);
  void close();

  void read(TableWorkbookDocumentPtr &workbook);
  void write(const TableWorkbookDocumentPtr &workbook);

  std::string file_path() const { return _file_path; }

private:
  void create_tables();
  void save_sheet(int id, const TableSheetPtr &sheet,
                  const TableWorkbookDocumentPtr &document);
  void save_cells(int id, const TableSheetPtr &sheet);
  void save_sheet_sizes(int id, const TableSheetPtr sheet);
  std::string quote(const std::string &s) const;
  void execute_sql(const std::string &sql);

  std::string lisp_value_type_to_string(LispValueType t) const;

  void read_tables();
  bool has_table(const std::string &name);

private:
  std::string _file_path;
  sqlite3 *_db;
  StringVector _tables;
};

#endif
