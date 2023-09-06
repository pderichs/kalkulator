#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include <cstddef>
#include <memory>

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_value.h"

class LispFunctionExecutionContext {
public:
  LispFunctionExecutionContext() = default;
  virtual ~LispFunctionExecutionContext() = default;
  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) = 0;

protected:
  LispValuePtrVector
  execute_functions(const LispValuePtrVector &params,
                    const LispExecutionContext &execution_context,
                    const std::any &context_param) const;
  LispValuePtrVector execute_functions_and_extract_list_results(
      const LispValuePtrVector &params,
      const LispExecutionContext &execution_context,
      const std::any &context_param) const;

  void ensure_params(const LispValuePtrVector &func) const;
  LispValuePtr expect_number(const LispValuePtr &value,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) const;
  LispValuePtr expect_parameter_at(const LispValuePtrVector &func,
                                   size_t index) const;

  LispValuePtrVector extract_params(const LispValuePtrVector& func) const;
};

#endif
