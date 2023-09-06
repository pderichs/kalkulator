#ifndef LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED

#include "../lisp/lisp_execution_context.h"
#include "../lisp/lisp_execution_context_error.h"
#include "../lisp/lisp_function_execution_context.h"
#include "table_workbook_document.h"
#include <any>
#include <sstream>

class LispExecutionContextCellReference : public LispFunctionExecutionContext {
public:
  LispExecutionContextCellReference(TableWorkbookDocumentPtr workbook) {
    _workbook = workbook;
  }

  virtual ~LispExecutionContextCellReference() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
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
    Location cell_location = std::any_cast<Location>(context_param);

    int row, col;

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    row = params[0]->to_integer();
    col = params[1]->to_integer();

    if (row == cell_location.y() && col == cell_location.x()) {
      // This would be a circular reference - cancel operation
      // throw LispExecutionContextError("Detected circular reference.");
      return LispValueFactory::new_string("#CIRCULARREFERR");
    }

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
