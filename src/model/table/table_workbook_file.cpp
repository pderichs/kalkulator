#include "table_workbook_file.h"
#include "table_cell.h"
#include "table_sheet.h"
#include "table_workbook_document.h"
#include "table_workbook_file_error.h"
#include <cstddef>
#include <filesystem>
#include <sstream>
#include <string>

static int read_sheet_callback(void *data, int argc, char **argv,
                               char **col_names) {
  TableWorkbookDocument *workbook = (TableWorkbookDocument *)data;

  std::string curr_sheet;
  std::string active_sheet;
  int r;
  int c;

  for (int i = 0; i < argc; i++) {
    std::string col(col_names[i]);
    std::string content;

    if (argv[i]) {
      content = argv[i];
    }

    if (col == "name") {
      workbook->add_sheet(content);
      curr_sheet = content;
    } else if (col == "active" && content == "1") {
      active_sheet = curr_sheet;
    } else if (col == "current_cell_row") {
      r = std::stoi(content);
    } else if (col == "current_cell_col") {
      c = std::stoi(content);
    }

    workbook->set_current_cell(curr_sheet, Location(c, r));
  }

  workbook->set_active_sheet(active_sheet);

  return 0;
}

static int read_cells_callback(void *data, int argc, char **argv,
                               char **col_names) {
  TableWorkbookDocument *workbook = (TableWorkbookDocument *)data;

  std::string curr_sheet;
  int r = 0;
  int c = 0;
  TableSheetPtr sheet;

  for (int i = 0; i < argc; i++) {
    std::string col(col_names[i]);
    std::string content;

    if (argv[i]) {
      content = argv[i];
    }

    if (col == "name") {
      sheet = workbook->table_sheet_by_name(content);
    } else if (col == "row") {
      r = std::stoi(content);
    } else if (col == "col") {
      c = std::stoi(content);
    } else if (col == "content") {
      const auto& opt_cell = sheet->get_cell(r, c);
      if (opt_cell) {
        const auto& cell = *opt_cell;
        if (cell) {
          workbook->update_cell_content(sheet, cell, content);
        }
      }
    }
  }

  return 0;
}
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

void TableWorkbookFile::read(TableWorkbookDocument &workbook) {
  workbook.clear();

  // Read sheets
  char *err_msg = nullptr;
  int rc = sqlite3_exec(_db, "SELECT * FROM sheets ORDER BY id ASC;",
                        read_sheet_callback, &workbook, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Error while reading sheet data: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }

  // Read cells
  err_msg = nullptr;
  rc = sqlite3_exec(_db,
                    "SELECT sheets.name, cells.row, cells.col, cells.content "
                    "FROM cells INNER JOIN sheets ON cells.sheet_id=sheets.id;",
                    read_cells_callback, &workbook, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Error while reading cells: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}

void TableWorkbookFile::write(const TableWorkbookDocument &workbook) {
  create_tables();

  execute_sql("DELETE FROM sheets;");
  execute_sql("DELETE FROM cells;");

  int id = 1;
  const auto &sheets = workbook.sheets();
  for (const auto &sheet : sheets) {
    save_sheet(id, sheet, workbook);
    id++;
  }
}

void TableWorkbookFile::create_tables() {
  char *err_msg = nullptr;

  const std::string sql =
      "CREATE TABLE IF NOT EXISTS meta ("
      "    property TEXT PRIMARY KEY NOT NULL,"
      "    value TEXT NOT NULL);"
      ""
      "CREATE TABLE IF NOT EXISTS sheets ("
      "    id INT PRIMARY KEY NOT NULL,"
      "    name TEXT NOT NULL,"
      "    current_cell_row INT NOT NULL,"
      "    current_cell_col INT NOT NULL,"
      "    active BOOL NOT NULL);"
      ""
      "CREATE TABLE IF NOT EXISTS cells ("
      "    sheet_id INT NOT NULL,"
      "    row INT NOT NULL,"
      "    col INT NOT NULL,"
      "    content TEXT NOT NULL,"
      ""
      "    FOREIGN KEY (sheet_id)"
      "    REFERENCES sheets (id)"
      "    ON DELETE CASCADE);"
      ""
      "CREATE UNIQUE INDEX IF NOT EXISTS idx_unique_cells ON cells(sheet_id, "
      "row, col);"
      ""
      "DELETE FROM meta;"
      "DELETE FROM sheets;"
      "DELETE FROM cells;"
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

void TableWorkbookFile::save_sheet(int id, const TableSheetPtr &sheet,
                                   const TableWorkbookDocument &document) {
  bool active = (sheet == document.current_sheet());

  std::stringstream sql;

  sql << "INSERT INTO sheets (id, name, current_cell_row, current_cell_col, "
         "active) ";
  sql << "VALUES (";
  sql << id << "," << quote(sheet->name) << ",";
  sql << sheet->current_cell.y() << "," << sheet->current_cell.x() << ",";

  if (active) {
    sql << "TRUE";
  } else {
    sql << "FALSE";
  }

  sql << ");";

  execute_sql(sql.str());

  save_cells(id, sheet);
}

std::string TableWorkbookFile::quote(const std::string &s) const {
  std::stringstream ss;
  ss << "'" << s << "'";
  return ss.str();
}

void TableWorkbookFile::execute_sql(const std::string &sql) {
  char *err_msg = nullptr;
  int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Unable to update file. SQL error: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}

void TableWorkbookFile::save_cells(int id, const TableSheetPtr &sheet) {
  std::stringstream ss;

  size_t rows = sheet->num_rows();
  size_t cols = sheet->num_cols();

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      const auto &opt_cell = sheet->get_cell(r, c);
      if (!opt_cell) {
        continue;
      }

      const auto &cell = *opt_cell;

      if (!cell || !cell->has_content()) {
        continue;
      }

      ss << "INSERT INTO cells (sheet_id, row, col, content) "
         << "VALUES (";
      ss << id << ", " << r << ", " << c << ", ";
      ss << quote(cell->get_formula_content()) << ");";
    }
  }

  execute_sql(ss.str());
}
