/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

  // Delete copy constructor and assignment operator
  TableWorkbookFile(const TableWorkbookFile &other) = delete;
  TableWorkbookFile &operator=(const TableWorkbookFile &other) = delete;

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
  void save_sheet_sizes(int id, const TableSheetPtr& sheet);
  static std::string quote(const std::string &s) ;
  void execute_sql(const std::string &sql);

  void read_tables();
  bool has_table(const std::string &name);

private:
  std::string _file_path;
  sqlite3 *_db;
  StringVector _tables;
};

#endif
