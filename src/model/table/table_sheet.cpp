#include "table_sheet.h"
#include "table_cell.h"
#include <cstddef>
#include <iostream>
#include <memory>

const size_t INITIAL_ROW_COUNT = 100;
const size_t INITIAL_COL_COUNT = 100;

TableSheet::TableSheet(const std::string &name) {
  std::cout << "Constructor table sheet" << std::endl;

  this->name = name;

  // Create initial rows and cols
  for (size_t r = 0; r < INITIAL_ROW_COUNT; r++) {
    auto row = std::make_shared<TableRow>();

    for (size_t c = 0; c < INITIAL_COL_COUNT; c++) {
      row->push_back(std::make_shared<TableCell>());
    }

    rows.push_back(row);
  }
}

TableSheet::~TableSheet() {

}
