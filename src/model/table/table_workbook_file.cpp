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

#include "table_workbook_file.h"
#include "table_cell.h"
#include "table_sheet.h"
#include "table_workbook_document.h"
#include "table_workbook_file_error.h"
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>

static int read_tables_callback(void *data, int argc, char **argv,
                                char **col_names) {
  auto tables = static_cast<StringVector *>(data);

  for (int i = 0; i < argc; i++) {
    std::string col(col_names[i]);

    if (col == "name" && argv[i]) {
      tables->push_back(argv[i]);
    }
  }

  return 0;
}

static int read_sheet_callback(void *data, int argc, char **argv,
                               char **col_names) {
  auto param = static_cast<
      std::tuple<TableWorkbookDocumentPtr, std::map<int, TableSheetPtr> *> *>(
      data);
  TableWorkbookDocumentPtr workbook = std::get<0>(*param);
  std::map<int, TableSheetPtr> *table_id_map = std::get<1>(*param);

  TableSheetPtr current_sheet_ptr;
  std::string curr_sheet;
  std::string active_sheet;
  int id = 0;
  int r;
  int c;

  for (int i = 0; i < argc; i++) {
    std::string col(col_names[i]);
    std::string content;

    if (argv[i]) {
      content = argv[i];
    }

    if (col == "name") {
      current_sheet_ptr = workbook->add_sheet(content);
      curr_sheet = content;
    } else if (col == "active" && content == "1") {
      active_sheet = curr_sheet;
    } else if (col == "current_cell_row") {
      r = std::stoi(content);
    } else if (col == "current_cell_col") {
      c = std::stoi(content);
    } else if (col == "id") {
      id = std::stoi(content);
    }
  }

  workbook->set_active_sheet(active_sheet);
  workbook->set_current_cell(curr_sheet, Location(c, r));

  assert(id > 0);
  (*table_id_map)[id] = current_sheet_ptr;

  return 0;
}

static int read_sheet_size_callback(void *data, int argc, char **argv,
                                    char **col_names) {
  auto param = static_cast<
      std::tuple<TableWorkbookDocumentPtr, std::map<int, TableSheetPtr> *> *>(
      data);
  TableWorkbookDocumentPtr workbook = std::get<0>(*param);
  std::map<int, TableSheetPtr> *table_id_map = std::get<1>(*param);

  TableSheetPtr referenced_sheet;
  int r = -1;
  int c = -1;
  int size = -1;

  for (int i = 0; i < argc; i++) {
    std::string col(col_names[i]);
    std::string content;

    if (argv[i]) {
      content = argv[i];
    }

    if (col == "sheet_id") {
      int id = std::stoi(content);
      auto it = table_id_map->find(id);
      if (it == table_id_map->end()) {
        throw std::runtime_error("Unexpected: Unknown sheet id");
      }

      referenced_sheet = it->second;
    } else if (col == "row") {
      if (!content.empty()) {
        r = std::stoi(content);
      }
    } else if (col == "col") {
      if (!content.empty()) {
        c = std::stoi(content);
      }
    } else if (col == "size") {
      size = std::stoi(content);
    }
  }

  assert(referenced_sheet);

  if (r != -1) {
    referenced_sheet->set_row_height(r, size);
  } else if (c != -1) {
    referenced_sheet->set_column_width(c, size);
  }

  return 0;
}

static int read_cells_callback(void *data, int argc, char **argv,
                               char **col_names) {
  auto param =
      static_cast<std::tuple<TableWorkbookDocument *, UpdateIdType> *>(data);
  auto *workbook = std::get<0>(*param);
  auto update_id = std::get<1>(*param);

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
      const auto &cell = sheet->get_cell(r, c);
      if (cell) {
        workbook->update_cell_content(sheet,
                                      Location(c, r),
                                      content,
                                      update_id);
      }
    }
  }

  return 0;
}

static int read_cell_formats_callback(void *data, int argc, char **argv,
                                      char **col_names) {
  auto param =
      static_cast<std::tuple<TableWorkbookDocument *, UpdateIdType> *>(data);
  auto *workbook = std::get<0>(*param);

  std::string curr_sheet;
  int r = 0;
  int c = 0;
  TableSheetPtr sheet;

  TableCellFormat format;

  bool background_present = false;
  bool foreground_present = false;

  TableCellColor background;
  TableCellColor foreground;

  for (int i = 0; i < argc; i++) {
    std::string col(col_names[i]);
    std::string content;

    if (argv[i]) {
      content = argv[i];
    }

    if (content.empty()) {
      continue;
    }

    if (col == "name") {
      sheet = workbook->table_sheet_by_name(content);
    } else if (col == "row") {
      r = std::stoi(content);
    } else if (col == "col") {
      c = std::stoi(content);
    } else if (col == "font_name") {
      format.font_name = content;
    } else if (col == "font_size") {
      format.font_size = std::stoul(content);
    } else if (col == "bold") {
      format.bold = std::stoi(content) != 0;
    } else if (col == "italic") {
      format.italic = std::stoi(content) != 0;
    } else if (col == "underlined") {
      format.underlined = std::stoi(content) != 0;
    } else if (col == "background_color_r") {
      background.r = std::stoi(content);
      background_present = true;
    } else if (col == "background_color_g") {
      background.g = std::stoi(content);
      background_present = true;
    } else if (col == "background_color_b") {
      background.b = std::stoi(content);
      background_present = true;
    } else if (col == "foreground_color_r") {
      foreground.r = std::stoi(content);
      foreground_present = true;
    } else if (col == "foreground_color_g") {
      foreground.g = std::stoi(content);
      foreground_present = true;
    } else if (col == "foreground_color_b") {
      foreground.b = std::stoi(content);
      foreground_present = true;
    }
  }

  if (background_present) {
    format.background_color = background;
  }

  if (foreground_present) {
    format.foreground_color = foreground;
  }

  auto cell = sheet->get_cell(r, c);
  cell->set_format(format);

  return 0;
}

static int read_cell_comments_callback(void *data, int argc, char **argv,
                                       char **col_names) {
  auto param =
      static_cast<std::tuple<TableWorkbookDocument *, UpdateIdType> *>(data);
  auto *workbook = std::get<0>(*param);

  std::string curr_sheet;
  int r = 0;
  int c = 0;
  TableSheetPtr sheet;

  TableCellComment comment;

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
    } else if (col == "comment") {
      comment.comment = content;
    }
  }

  auto cell = sheet->get_cell(r, c);
  cell->set_comment(comment);

  return 0;
}
TableWorkbookFile::TableWorkbookFile()
    : _file_path(), _db(nullptr), _tables() {}

TableWorkbookFile::~TableWorkbookFile() { close(); }

void TableWorkbookFile::open(const std::string &filename) {
  close();

  int rc = sqlite3_open(filename.c_str(), &_db);
  if (rc) {
    std::stringstream ss;
    ss << "Unable to open workbook file: " << sqlite3_errmsg(_db);
    _db = nullptr;
    throw TableWorkbookFileError(ss.str());
  }

  _file_path = filename;
}

void TableWorkbookFile::close() {
  if (_db) {
    sqlite3_close(_db);
    _db = nullptr;
  }
}

void TableWorkbookFile::read(TableWorkbookDocumentPtr &workbook) {
  workbook->clear();

  char *err_msg = nullptr;

  read_tables();

  if (!has_table("meta") || !has_table("sheets") || !has_table("cells")) {
    throw TableWorkbookFileError("Invalid file structure.");
  }

  std::map<int, TableSheetPtr> table_id_map;

  // Read sheets
  auto param = std::make_tuple(workbook, &table_id_map);
  int rc = sqlite3_exec(_db, "SELECT * FROM sheets ORDER BY id ASC;",
                        read_sheet_callback, (void *)&param, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Error while reading sheet data: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }

  // Read column and row sizes
  if (has_table("sheet_sizes")) {
    param = std::make_tuple(workbook, &table_id_map);
    rc = sqlite3_exec(_db, "SELECT * FROM sheet_sizes;",
                      read_sheet_size_callback, (void *)&param, &err_msg);

    if (rc != SQLITE_OK) {
      std::stringstream ss;
      ss << "Error while reading sheet sizes data: " << err_msg;
      sqlite3_free(err_msg);
      throw TableWorkbookFileError(ss.str());
    }
  }

  // Read cells
  auto cells_param = std::make_tuple(workbook, generate_update_id());

  err_msg = nullptr;
  rc = sqlite3_exec(_db,
                    "SELECT sheets.name, cells.row, cells.col, cells.content "
                    "FROM cells INNER JOIN sheets ON cells.sheet_id=sheets.id;",
                    read_cells_callback, (void *)&cells_param, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Error while reading cells: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }

  // Read formats
  if (has_table("cell_formats")) {
    err_msg = nullptr;
    rc = sqlite3_exec(_db,
                      "SELECT sheets.name, cell_formats.row, cell_formats.col, "
                      "cell_formats.font_name,  cell_formats.font_size,  "
                      "cell_formats.bold,  cell_formats.italic,  cell_formats.underlined, "
                      "cell_formats.background_color_r, cell_formats.background_color_g, cell_formats.background_color_b, "
                      "cell_formats.foreground_color_r, cell_formats.foreground_color_g, cell_formats.foreground_color_b "
                      "FROM cell_formats INNER JOIN sheets ON cell_formats.sheet_id=sheets.id;",
                      read_cell_formats_callback,
                      (void *)&cells_param,
                      &err_msg);

    if (rc != SQLITE_OK) {
      std::stringstream ss;
      ss << "Error while reading cell formats: " << err_msg;
      sqlite3_free(err_msg);
      throw TableWorkbookFileError(ss.str());
    }
  }

  // Read comments
  if (has_table("cell_comments")) {
    err_msg = nullptr;
    rc = sqlite3_exec(_db,
                      "SELECT sheets.name, cell_comments.row, cell_comments.col, cell_comments.comment "
                      "FROM cell_comments INNER JOIN sheets ON cell_comments.sheet_id=sheets.id;",
                      read_cell_comments_callback,
                      (void *)&cells_param,
                      &err_msg);

    if (rc != SQLITE_OK) {
      std::stringstream ss;
      ss << "Error while reading cell comments: " << err_msg;
      sqlite3_free(err_msg);
      throw TableWorkbookFileError(ss.str());
    }
  }
}

void TableWorkbookFile::save_sheet_sizes(int id, const TableSheetPtr &sheet) {
  size_t s = 0;
  for (const auto &col_def : sheet->column_definitions()) {
    if (col_def->width != DEFAULT_COLUMN_WIDTH) {
      std::stringstream sql;
      sql << "INSERT INTO sheet_sizes (sheet_id, row, col, size)"
          << "VALUES (";
      sql << id << ", NULL, " << s << "," << col_def->width;
      sql << ");";

      execute_sql(sql.str());
    }

    s++;
  }

  s = 0;
  for (const auto &row_def : sheet->row_definitions()) {
    if (row_def->height != DEFAULT_ROW_HEIGHT) {
      std::stringstream sql;
      sql << "INSERT INTO sheet_sizes (sheet_id, row, col, size)"
          << "VALUES (";
      sql << id << ", " << s << ", NULL, " << row_def->height;
      sql << ");";

      execute_sql(sql.str());
    }

    s++;
  }
}

void TableWorkbookFile::write(const TableWorkbookDocumentPtr &workbook) {
  create_tables();

  int id = 1;
  const auto &sheets = workbook->sheets();
  for (const auto &sheet : sheets) {
    save_sheet_sizes(id, sheet);
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
      "CREATE TABLE IF NOT EXISTS sheet_sizes ("
      "    sheet_id INT NOT NULL,"
      "    row INT,"
      "    col INT,"
      "    size INT NOT NULL,"
      ""
      "    FOREIGN KEY (sheet_id)"
      "    REFERENCES sheets (id)"
      "    ON DELETE CASCADE);"
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
      "CREATE TABLE IF NOT EXISTS cell_formats ("
      "    sheet_id INT NOT NULL,"
      "    row INT NOT NULL,"
      "    col INT NOT NULL,"
      "    font_name TEXT,"
      "    font_size INT,"
      "    bold INT,"
      "    italic INT,"
      "    underlined INT,"
      "    background_color_r INT,"
      "    background_color_g INT,"
      "    background_color_b INT,"
      "    foreground_color_r INT,"
      "    foreground_color_g INT,"
      "    foreground_color_b INT,"
      ""
      "    FOREIGN KEY (sheet_id)"
      "    REFERENCES sheets (id)"
      "    ON DELETE CASCADE);"
      ""
      "CREATE TABLE IF NOT EXISTS cell_comments ("
      "    sheet_id INT NOT NULL,"
      "    row INT NOT NULL,"
      "    col INT NOT NULL,"
      "    comment TEXT NOT NULL,"
      ""
      "    FOREIGN KEY (sheet_id)"
      "    REFERENCES sheets (id)"
      "    ON DELETE CASCADE);"
      ""
      "CREATE UNIQUE INDEX IF NOT EXISTS idx_unique_cells ON cells(sheet_id, "
      "row, col);"
      ""
      "CREATE UNIQUE INDEX IF NOT EXISTS idx_unique_cell_formats ON "
      "cell_formats(sheet_id, row, col);"
      ""
      "CREATE UNIQUE INDEX IF NOT EXISTS idx_unique_cell_comments ON "
      "cell_comments(sheet_id, row, col);"
      ""
      "DELETE FROM meta;"
      "DELETE FROM sheets;"
      "DELETE FROM cells;"
      "DELETE FROM sheet_sizes;"
      "DELETE FROM cell_formats;"
      "DELETE FROM cell_comments;"
      ""
      "INSERT INTO meta (property, value) VALUES ('version', '0.0.3a');";

  int rc = sqlite3_exec(_db, sql.c_str(), nullptr, nullptr, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Unable to write initial file structure. SQL error: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}

void TableWorkbookFile::save_sheet(int id, const TableSheetPtr &sheet,
                                   const TableWorkbookDocumentPtr &document) {
  bool active = (sheet == document->current_sheet());

  std::string
      sql = "INSERT INTO sheets (id, name, current_cell_row, current_cell_col, "
            "active) VALUES (?, ?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(_db, sql.data(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    throw TableWorkbookFileError("Unable to prepare statement (format)");
  }

  sqlite3_bind_int(stmt, 1, id);
  sqlite3_bind_text(stmt, 2, sheet->name().data(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, sheet->selection().primary_y());
  sqlite3_bind_int(stmt, 4, sheet->selection().primary_x());
  sqlite3_bind_int(stmt, 5, active ? 1 : 0);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);

    throw TableWorkbookFileError("Unable to write sheet");
  }

  sqlite3_finalize(stmt);

  save_cells(id, sheet);
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

  size_t rows = sheet->row_count();
  size_t cols = sheet->col_count();

  std::string sql = "INSERT INTO cells (sheet_id, row, col, content) "
                    "VALUES (?, ?, ?, ?);";

  sqlite3_stmt *stmt;

  for (size_t r = 0; r < rows; r++) {
    for (size_t c = 0; c < cols; c++) {
      const auto &cell = sheet->get_cell(r, c);
      if (!cell || !cell->has_content()) {
        continue;
      }

      int rc = sqlite3_prepare_v2(_db, sql.data(), -1, &stmt, NULL);
      if (rc != SQLITE_OK) {
        throw TableWorkbookFileError("Unable to prepare statement (save cells)");
      }

      sqlite3_bind_int(stmt, 1, id);
      sqlite3_bind_int(stmt, 2, r);
      sqlite3_bind_int(stmt, 3, c);
      sqlite3_bind_text(stmt,
                        4,
                        cell->get_formula_content().data(),
                        -1,
                        SQLITE_STATIC);

      rc = sqlite3_step(stmt);
      if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);

        throw TableWorkbookFileError("Unable to write cell");
      }

      sqlite3_finalize(stmt);

      save_cell_format(id, cell);
      save_cell_comment(id, cell);
    }
  }
}

void TableWorkbookFile::read_tables() {
  _tables.clear();

  char *err_msg;

  int rc =
      sqlite3_exec(_db, "SELECT name FROM sqlite_schema WHERE type='table';",
                   read_tables_callback, (void *)&_tables, &err_msg);

  if (rc != SQLITE_OK) {
    std::stringstream ss;
    ss << "Error while reading tables: " << err_msg;
    sqlite3_free(err_msg);
    throw TableWorkbookFileError(ss.str());
  }
}

bool TableWorkbookFile::has_table(const std::string &name) {
  return std::find(_tables.begin(), _tables.end(), name) != _tables.end();
}

void TableWorkbookFile::save_cell_format(int sheet_id,
                                         const TableCellPtr &cell) {
  if (!cell->has_format()) {
    return;
  }

  const TableCellFormat &format = cell->format();

  std::string sql = "INSERT INTO cell_formats (sheet_id, row, col, "
                    "font_name, font_size, "
                    "bold, italic, underlined, "
                    "background_color_r, background_color_g, background_color_b, "
                    "foreground_color_r, foreground_color_g, foreground_color_b) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(_db, sql.data(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_finalize(stmt);

    throw TableWorkbookFileError("Unable to prepare statement (format)");
  }

  sqlite3_bind_int(stmt, 1, sheet_id);
  sqlite3_bind_int(stmt, 2, cell->row());
  sqlite3_bind_int(stmt, 3, cell->col());

  if (format.font_name) {
    sqlite3_bind_text(stmt, 4, (*format.font_name).data(), -1, SQLITE_STATIC);
  } else {
    sqlite3_bind_null(stmt, 4);
  }

  if (format.font_size) {
    sqlite3_bind_int(stmt, 5, *format.font_size);
  } else {
    sqlite3_bind_null(stmt, 5);
  }

  if (format.bold) {
    sqlite3_bind_int(stmt, 6, (*format.bold) ? 1 : 0);
  } else {
    sqlite3_bind_null(stmt, 6);
  }

  if (format.italic) {
    sqlite3_bind_int(stmt, 7, (*format.italic) ? 1 : 0);
  } else {
    sqlite3_bind_null(stmt, 7);
  }

  if (format.underlined) {
    sqlite3_bind_int(stmt, 8, (*format.underlined) ? 1 : 0);
  } else {
    sqlite3_bind_null(stmt, 8);
  }

  if (format.background_color) {
    sqlite3_bind_int(stmt, 9, (*format.background_color).r);
    sqlite3_bind_int(stmt, 10, (*format.background_color).g);
    sqlite3_bind_int(stmt, 11, (*format.background_color).b);
  } else {
    sqlite3_bind_null(stmt, 9);
    sqlite3_bind_null(stmt, 10);
    sqlite3_bind_null(stmt, 11);
  }

  if (format.foreground_color) {
    sqlite3_bind_int(stmt, 12, (*format.foreground_color).r);
    sqlite3_bind_int(stmt, 13, (*format.foreground_color).g);
    sqlite3_bind_int(stmt, 14, (*format.foreground_color).b);
  } else {
    sqlite3_bind_null(stmt, 12);
    sqlite3_bind_null(stmt, 13);
    sqlite3_bind_null(stmt, 14);
  }

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);

    std::cerr << "ERROR: " << rc << std::endl;

    throw TableWorkbookFileError("Unable to write format");
  }

  sqlite3_finalize(stmt);
}

void TableWorkbookFile::save_cell_comment(int sheet_id,
                                          const TableCellPtr &cell) {
  if (!cell->has_comment()) {
    return;
  }

  const TableCellComment &comment = cell->comment();

  std::string sql = "INSERT INTO cell_comments (sheet_id, row, col, comment) "
                    "VALUES (?, ?, ?, ?);";

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(_db, sql.data(), -1, &stmt, NULL);
  if (rc != SQLITE_OK) {
    sqlite3_finalize(stmt);

    throw TableWorkbookFileError("Unable to prepare statement (comment)");
  }

  sqlite3_bind_int(stmt, 1, sheet_id);
  sqlite3_bind_int(stmt, 2, cell->row());
  sqlite3_bind_int(stmt, 3, cell->col());
  sqlite3_bind_text(stmt, 4, (comment.comment).data(), -1, SQLITE_STATIC);

  rc = sqlite3_step(stmt);
  if (rc != SQLITE_DONE) {
    sqlite3_finalize(stmt);

    std::cerr << "ERROR: " << rc << std::endl;

    throw TableWorkbookFileError("Unable to write comment");
  }

  sqlite3_finalize(stmt);
}
