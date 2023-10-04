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

#ifndef LISP_EXECUTION_CONTEXT_CELL_RANGE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_RANGE_INCLUDED

#include "../lisp/lisp_function_execution_context.h"
#include "../lisp/lisp_value_factory.h"
#include "../lisp/lisp_common_values.h"
#include "../lisp/value_converter.h"
#include "table_workbook_document.h"
#include <any>
#include <sstream>

class LispExecutionContextCellRange : public LispFunctionExecutionContext {
public:
  explicit LispExecutionContextCellRange(TableWorkbookDocumentPtr document)
      : _document(document) {}
  ~LispExecutionContextCellRange() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    ensure_params(func);

    LispValuePtrVector params =
        extract_and_execute_params(func,
                                   execution_context,
                                   context_param,
                                   update_id);

    if (params.size() != 4) {
      std::stringstream ss;
      ss << "Parameter count mismatch for cell_range: Expected 4 but got "
         << params.size();
      throw LispExecutionContextError(ss.str());
    }

    auto this_cell = std::any_cast<TableCellLocation>(context_param);
    auto sheet = this_cell.sheet();

    LispValuePtrVector result;

    LispValue::IntegerType ranges[4]; // from_row, from_col, to_row, to_col
    int n = 0;
    for (const auto &param : params) {
      LispValuePtr value(
          expect_number(param, execution_context, context_param, update_id));
      ranges[n] = value->to_integer();
      n++;
    }

    Location from(ranges[1], ranges[0]);
    Location to(ranges[3], ranges[2]);

    TableCellPtrVector cells = _document->get_range(from, to);

    for (const auto &cell : cells) {
      if (!cell) {
        continue;
      }

      if (cell->row() == this_cell.location().y()
          && cell->col() == this_cell.location().x()) {
        return LispCommonValues::error_circular_ref();
      }

      // Inform document about cell reference
      _document->add_update_listener(this_cell,
                                     TableCellLocation(sheet,
                                                       cell->location()));

      LispValuePtr value(cell->result());
      if (value && !value->is_none()) {
        result.push_back(value);
      }
    }

    return LispValueFactory::new_list(result);
  }

private:
  TableWorkbookDocumentPtr _document;
};

#endif
