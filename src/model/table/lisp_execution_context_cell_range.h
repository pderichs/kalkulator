#ifndef LISP_EXECUTION_CONTEXT_CELL_RANGE_INCLUDED
#define LISP_EXECUTION_CONTEXT_CELL_RANGE_INCLUDED

#include "../lisp/lisp_function_execution_context.h"
#include "table_workbook_document.h"
#include <sstream>
#include <variant>

class LispExecutionContextCellRange : public LispFunctionExecutionContext {
public:
  LispExecutionContextCellRange(TableWorkbookDocument *document) {
    _document = document;
  }

  virtual ~LispExecutionContextCellRange() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context) {
    ensure_params(func);

    LispValuePtrVector result;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    if (params.size() != 4) {
      std::stringstream ss;
      ss << "Parameter count mismatch for cell_range: Expected 4 but got "
         << params.size();
      throw LispExecutionContextError(ss.str());
    }

    int ranges[4]; // from_row, from_col, to_row, to_col
    int n = 0;
    for (const auto &param : params) {
      LispValue value(expect_number(param, execution_context));
      ranges[n] = (int)value.number();
      n++;
    }

    // From smaller location to higher location - reorder
    // if necessary
    if (ranges[0] > ranges[2] || ranges[1] > ranges[3]) {
      std::swap(ranges[0], ranges[2]);
      std::swap(ranges[1], ranges[3]);
    }

    Location from(ranges[1], ranges[0]);
    Location to(ranges[3], ranges[2]);

    TableCellPtrVector cells = _document->get_range(from, to);

    for (const auto &cell : cells) {
      if (!cell) {
        continue;
      }

      LispValuePtr value(cell->lisp_value());
      if (value && !value->is_none()) {
        result.push_back(value);
      }
    }

    return LispValue(result);
  }

private:
  TableWorkbookDocument *_document;
};

#endif
