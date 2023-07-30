#include "table_workbook_file.h"
#include "table_workbook_file_error.h"
#include <filesystem>
#include <sstream>

TableWorkbookFile::TableWorkbookFile() { _db = nullptr; }

TableWorkbookFile::~TableWorkbookFile() { close(); }

void TableWorkbookFile::open(const std::string &filename) {
  close();

  _needs_setup = !std::filesystem::exists(filename);
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
  if (_needs_setup) {
    create_tables();
  }
}

void TableWorkbookFile::create_tables() {
  char *err_msg = nullptr;

  const std::string sql = "create table meta ("
                          "    property text not null,"
                          "    value text not null );"
                          ""
                          "create table sheets ("
                          "    id int primary key not null,"
                          "    name text not null );"
                          ""
                          "create table cells ("
                          "    sheet_id int not null,"
                          "    row int not null,"
                          "    col int not null,"
                          "    visible_content text not null,"
                          "    formula text not null );"
                          ""
                          "INSERT INTO meta (property, value) VALUES ('version', '0.1a');";

  int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Unable to write initial file structure. SQL error: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}
