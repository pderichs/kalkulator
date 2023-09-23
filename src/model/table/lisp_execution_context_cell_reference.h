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
#include "table_workbook_document.h"
#include <any>
#include <sstream>

class LispExecutionContextCellReference : public LispFunctionExecutionContext {
public:
  explicit LispExecutionContextCellReference(TableWorkbookDocumentPtr workbook) : _workbook(workbook) {}
  ~LispExecutionContextCellReference() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    LispValuePtrVector params = extract_params(func);

    if (params.size() != 2) {
      throw LispExecutionContextError(
          "cell function needs 2 parameters (row and colum)");
    }

    // To prevent a circular reference we need to check whether
    // the function parameters would make the cell reference
    // itself.
    // The context_param contains the current cell location
    // within the spreadsheet calculation application.
    auto cell_location = std::any_cast<Location>(context_param);

    LispValue::IntegerType row, col;

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    row = params[0]->to_integer();
    col = params[1]->to_integer();

    if (row == cell_location.y() && col == cell_location.x()) {
      // This would be a circular reference - cancel operation
      // throw LispExecutionContextError("Detected circular reference.");
      return LispValueFactory::new_string("#CIRCULARREFERR");
    }

    // Inform document about cell reference
    std::cerr << "Cell " << cell_location << " is listening to updates from " << col << ", " << row << std::endl;
    _workbook->add_update_listener(cell_location, Location(col, row));

    auto opt_cell = _workbook->get_cell(Location(col, row));
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
  TableWorkbookDocumentPtr _workbook;
};

#endif
