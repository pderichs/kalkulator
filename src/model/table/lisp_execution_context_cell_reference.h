#ifndef LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_REFERENCE_INCLUDED

#include "../lisp/lisp_execution_context.h"
#include "../lisp/lisp_execution_context_error.h"
#include <sstream>

class LispExecutionContextCellReference : public LispFunctionExecutionContext {
  LispExecutionContextCellReference() = default;
  virtual ~LispExecutionContextCellReference() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context) {
    if (func.param_count() != 2) {
      throw LispExecutionContextError(
          "cell function needs 2 parameters (row and colum)");
    }

    int row, col;

    auto opt_row_param = func.param_at(0);
    auto opt_col_param = func.param_at(1);

    if (!opt_col_param || !opt_row_param) {
      throw LispExecutionContextError(
          "cell: Unable to retrieve one of needed parameters");
    }

    auto row_param = *opt_row_param;
    auto col_param = *opt_col_param;

    LispValue row_value;
    if (row_param->is_function()) {
      row_value = execution_context.execute(*row_param);
    } else if (row_param->is_number()) {
      row_value = *row_param;
    } else {
      std::stringstream ss;
      ss << "Unable to retrieve row value from param of type "
         << (int)row_param->type();
      throw LispExecutionContextError(ss.str());
    }

    LispValue col_value;
    if (col_param->is_function()) {
      col_value = execution_context.execute(*col_param);
    } else if (col_param->is_number()) {
      col_value = *col_param;
    } else {
      std::stringstream ss;
      ss << "Unable to retrieve col value from param of type "
         << (int)col_param->type();
      throw LispExecutionContextError(ss.str());
    }

    row = (int)row_value.number();
    col = (int)col_value.number();

    // TODO retrieve value from cell

    return LispValue();
  }
};

#endif
