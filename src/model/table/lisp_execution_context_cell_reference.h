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

#ifndef LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED

#include "../lisp/lisp_execution_context.h"
#include "../lisp/lisp_execution_context_error.h"
#include "../lisp/lisp_function_execution_context.h"
#include "../lisp/lisp_value_factory.h"
#include "../lisp/lisp_common_values.h"
#include "table_workbook_document.h"
#include <any>
#include <sstream>

class LispExecutionContextCellReference : public LispFunctionExecutionContext {
public:
  explicit LispExecutionContextCellReference(TableWorkbookDocumentPtr workbook)
      : _document(workbook) {}
  ~LispExecutionContextCellReference() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    LispValuePtrVector params =
        extract_and_execute_params(func, execution_context, context_param);

    if (params.size() < 2 || params.size() > 3) {
      return LispCommonValues::error_parameter_count();
    }

    std::string sheet;
    size_t row_index, col_index;

    if (params[0]->is_string()) {
      // We have a sheet reference
      row_index = 1;
      col_index = 2;

      sheet = params[0]->string();
    } else {
      row_index = 0;
      col_index = 1;
    }

    if (!params[row_index]->is_number() || !params[col_index]->is_number()) {
      return LispCommonValues::error_parameter();
    }

    // To prevent a circular reference we need to check whether
    // the function parameters would make the cell reference
    // itself.
    // The context_param contains the current cell location
    // within the spreadsheet calculation application.
    auto source_cell_location = std::any_cast<Location>(context_param);

    auto referenced_cell_location =
        read_cell_location(params, row_index, col_index);

    if (source_cell_location == referenced_cell_location) {
      // This would be a circular reference - cancel operation
      // throw LispExecutionContextError("Detected circular reference.");
      return LispCommonValues::error_circular_ref();
    }

    // Inform document about cell reference
    // std::cerr << "Cell " << source_cell_location << " is listening to updates from "
    // << col << ", " << row << std::endl;
    _document->add_update_listener(source_cell_location,
                                   referenced_cell_location);

    auto opt_cell = _document->get_cell(referenced_cell_location, sheet);
    if (!opt_cell) {
      return LispValueFactory::new_none();
    }

    auto cell = *opt_cell;

    if (!cell || !cell->has_content()) {
      return LispValueFactory::new_none();
    }

    return cell->lisp_value();
  }

private:
  Location read_cell_location(const LispValuePtrVector &params,
                              size_t row_index,
                              int col_index) const {
    LispValue::IntegerType row, col;

    row = params[row_index]->to_integer();
    col = params[col_index]->to_integer();

    return Location(col, row);
  }

private:
  TableWorkbookDocumentPtr _document;
};

#endif
