#ifndef LISP_EXECUTION_CONTEXT_IF_INCLUDED
#define LISP_EXECUTION_CONTEXT_IF_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include <cstddef>
#include <memory>

class LispExecutionContextIf : public LispFunctionExecutionContext {
public:
  LispExecutionContextIf() = default;
  virtual ~LispExecutionContextIf() = default;

  virtual LispValue value(const LispFunction &func,
                          const LispExecutionContext &execution_context,
                          const std::any &context_param) {
    if (func.param_count() < 2 || func.param_count() > 3) {
      throw LispExecutionContextError("Unexpected parameter count for if");
    }

    LispValuePtr condition = expect_parameter_at(func, 0);

    if (condition->is_function()) {
      condition = std::make_shared<LispValue>(
          execution_context.execute(*condition, context_param));
    }

    LispValuePtr result;

    if (condition->is_truthy()) {
      result = execute_result_param(func, 1, execution_context, context_param);
    } else {
      result = execute_result_param(func, 2, execution_context, context_param);
    }

    return *result;
  }

  LispValuePtr
  execute_result_param(const LispFunction &func, size_t index,
                       const LispExecutionContext &execution_context,
                       const std::any &context_param) const {
    LispValuePtr result;

    const auto &result_param = expect_parameter_at(func, index);

    if (result_param->is_function()) {
      result = std::make_shared<LispValue>(
          execution_context.execute(*result_param, context_param));
    } else {
      result = result_param;
    }

    return result;
  }
};

#endif
