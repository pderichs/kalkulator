#ifndef LISP_EXECUTION_CONTEXT_CELL_RANGE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_RANGE_INCLUDED

#include "../lisp/lisp_function_execution_context.h"
#include "../lisp/lisp_value_factory.h"
#include "table_workbook_document.h"
#include <sstream>
#include <variant>

class LispExecutionContextCellRange : public LispFunctionExecutionContext {
public:
  LispExecutionContextCellRange(TableWorkbookDocumentPtr document) {
    _document = document;
  }

  virtual ~LispExecutionContextCellRange() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    ensure_params(func);

    Location this_cell = std::any_cast<Location>(context_param);

    LispValuePtrVector result;

    LispValuePtrVector params = extract_params(func);

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    if (params.size() != 4) {
      std::stringstream ss;
      ss << "Parameter count mismatch for cell_range: Expected 4 but got "
         << params.size();
      throw LispExecutionContextError(ss.str());
    }

    int ranges[4]; // from_row, from_col, to_row, to_col
    int n = 0;
    for (const auto &param : params) {
      LispValuePtr value(
          expect_number(param, execution_context, context_param));
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

      if (cell->row() == this_cell.y() && cell->col() == this_cell.x()) {
        return LispValueFactory::new_string("#CIRCULARREFERR");
      }

      LispValuePtr value(cell->lisp_value());
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
