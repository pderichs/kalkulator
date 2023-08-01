#ifndef LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED

#include "../lisp/lisp_execution_context.h"
#include "../lisp/lisp_execution_context_error.h"
#include "../lisp/lisp_function_execution_context.h"
#include "table_workbook_document.h"
#include <sstream>

class LispExecutionContextCellReference : public LispFunctionExecutionContext {
public:
  LispExecutionContextCellReference(TableWorkbookDocument *pWorkbook) {
    _workbook = pWorkbook;
  }

  virtual ~LispExecutionContextCellReference() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    if (func.param_count() != 2) {
      throw LispExecutionContextError(
          "cell function needs 2 parameters (row and colum)");
    }

    int row, col;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    row = (int)params[0]->number();
    col = (int)params[1]->number();

    auto opt_cell = _workbook->get_cell(Location(col, row));
    if (!opt_cell) {
      return LispValue();
    }

    auto cell = *opt_cell;

    if (!cell) {
      return LispValue();
    }

    return *(cell->lisp_value());
  }

private:
  TableWorkbookDocument *_workbook;
};

#endif
