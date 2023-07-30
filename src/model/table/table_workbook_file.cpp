#include "table_workbook_file.h"
#include "table_workbook_file_error.h"
#include <filesystem>
#include <sstream>

TableWorkbookFile::TableWorkbookFile() { _db = nullptr; }

TableWorkbookFile::~TableWorkbookFile() { close(); }

void TableWorkbookFile::open(const std::string &filename) {
  close();

  //_needs_setup = !std::filesystem::exists(filename);
  // TODO If file exists, also check meta / version to
  // verify further need for update.

  int rc = sqlite3_open(filename.c_str(), &_db);
  if (rc) {
    std::stringstream ss;
    ss << "Unable to open workbook file: " << sqlite3_errmsg(_db);
    _db = nullptr;
    throw TableWorkbookFileError(ss.str());
  }

  _filename = filename;
}

void TableWorkbookFile::close() {
  if (_db) {
    sqlite3_close(_db);
    _db = nullptr;
  }
}

void TableWorkbookFile::read(TableWorkbookDocument &workbook) {}

void TableWorkbookFile::write(const TableWorkbookDocument &workbook) {
  create_tables();

  execute_sql("DELETE FROM sheets;");
  execute_sql("DELETE FROM cells;");

  int id = 1;
  const auto& sheets = workbook.sheets();
  for (const auto& sheet : sheets) {
    save_sheet(id, sheet);
    id++;
  }
}

void TableWorkbookFile::create_tables() {
  char *err_msg = nullptr;

  const std::string sql =
      "create table if not exists meta ("
      "    property text primary key not null,"
      "    value text not null );"
      ""
      "create table if not exists sheets ("
      "    id int primary key not null,"
      "    name text not null );"
      ""
      "create table if not exists cells ("
      "    sheet_id int not null,"
      "    row int not null,"
      "    col int not null,"
      "    visible_content text not null,"
      "    formula text not null );"
      ""
      "create unique index if not exists idx_unique_cells on cells(sheet_id, "
      "row, col);"
      ""
      "INSERT INTO meta (property, value) VALUES ('version', '0.0.1a');";

  int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Unable to write initial file structure. SQL error: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}

void TableWorkbookFile::save_sheet(int id, const TableSheetPtr &sheet) {
  std::stringstream sql;

  sql << "INSERT INTO sheets (id, name, current_cell_row, current_cell_col) ";
  sql << "VALUES (";
  sql << id << "," << quote(sheet->name) << ",";
  sql << sheet->current_cell.y() << "," << sheet->current_cell.x() << ");";

  execute_sql(sql.str());
}

std::string TableWorkbookFile::quote(const std::string& s) const {
  std::stringstream ss;
  ss << "'" << s << "'";
  return ss.str();
}

void TableWorkbookFile::execute_sql(const std::string& sql) {
  char *err_msg = nullptr;
  int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Unable to update file. SQL error: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}
