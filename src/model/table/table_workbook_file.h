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

/**
 * Provides functionality to store and read table workbook files.
 */
class TableWorkbookFile {
public:
  TableWorkbookFile();
  ~TableWorkbookFile();

  // Delete copy constructor and assignment operator
  TableWorkbookFile(const TableWorkbookFile &other) = delete;
  TableWorkbookFile &operator=(const TableWorkbookFile &other) = delete;

  /**
   * Opens a workbook file.
   * @param filename Path of the file to be opened
   */
  void open(const std::string &filename);

  /**
   * Closes the workbook file.
   */
  void close();

  /**
   * Read workbook data from the current opened file.
   * @param workbook Destination of the data read from file
   */
  void read(TableWorkbookDocumentPtr &workbook);

  /**
   * Writes the workbook data to the current opened file.
   * @param workbook Workbook to be stored within the file.
   */
  void write(const TableWorkbookDocumentPtr &workbook);

  /**
   * @return the current file path
   */
  std::string file_path() const { return _file_path; }

private:
  /**
   * Creates the SQLite tables to store workbook information within
   * the file.
   */
  void create_tables();

  /**
   * Saves a specific sheet to the file
   *
   * @param id Sheet id
   * @param sheet Sheet instance to be stored
   * @param document Workbook document instance used to identify active sheet
   */
  void save_sheet(int id, const TableSheetPtr &sheet,
                  const TableWorkbookDocumentPtr &document);

  /**
   * Stores the cells of a sheet to the file.
   *
   * @param id Sheet id
   * @param sheet Cells of this sheet will be stored
   */
  void save_cells(int id, const TableSheetPtr &sheet);

  /**
   * Stores the row and column sizes of the given sheet to the file.
   *
   * @param id Sheet id
   * @param sheet Sheet which contains the row and column sizes to be stored
   */
  void save_sheet_sizes(int id, const TableSheetPtr &sheet);

  /**
   * Helper function to quote contents of a string
   * @param s String to be quoted
   * @return Quoted string
   */
  static std::string quote(const std::string &s);

  /**
   * Executes a SQL statement
   * @param sql Statement to be executed
   */
  void execute_sql(const std::string &sql);

  /**
   * Reads the tables data from the file.
   */
  void read_tables();

  /**
   * Checks whether the given table name is present within the file
   * @param name Name of table to query
   * @return true if table exists, false otherwise
   */
  bool has_table(const std::string &name);

private:
  std::string _file_path;
  sqlite3 *_db;
  StringVector _tables;
  void save_cell_format(int id, const TableCellPtr &shared_ptr);
  void save_cell_comment(int id, const TableCellPtr &shared_ptr);
};

#endif
