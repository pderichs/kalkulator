#ifndef LISP_EXECUTION_CONTEXT_AVG_INCLUDED
#define LISP_EXECUTION_CONTEXT_AVG_INCLUDED

#include "../lisp/lisp_function_execution_context.h"
#include "table_workbook_document.h"
#include <cstddef>

class LispExecutionContextAvg : public LispFunctionExecutionContext {
private:
  TableWorkbookDocument *_document;

public:
  LispExecutionContextAvg(TableWorkbookDocument *document) {
    _document = document;
  }

  virtual ~LispExecutionContextAvg() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param = {}) {
    ensure_params(func);

    double result = 0.0;

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context);

    size_t param_count = params.size();

    for (const auto &param : params) {
      LispValue value(expect_number(param, execution_context));
      result += value.number();
    }

    return LispValue(result / param_count);
  }
};

#endif
