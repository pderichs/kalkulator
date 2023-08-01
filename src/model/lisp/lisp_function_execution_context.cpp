#include "lisp_function_execution_context.h"
#include "lisp_execution_context.h"
#include "lisp_function.h"
#include "lisp_value.h"
#include <memory>
#include <sstream>

void LispFunctionExecutionContext::ensure_params(
    const LispFunction &func) const {
  if (func.param_count() == 0) {
    throw LispExecutionContextError("Unexpected: No params given");
  }
}

LispValue LispFunctionExecutionContext::expect_number(
    const LispValuePtr &value, const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValue result;

  if (value->is_function()) {
    result = execution_context.execute(*value, context_param);
  } else if (value->is_number()) {
    result = *value;
  } else {
    std::stringstream ss;
    ss << "Unable to perform subtraction with this lisp value "
       << (int)value->type();
    throw LispExecutionContextError(ss.str());
  }

  return result;
}

LispValuePtrVector
LispFunctionExecutionContext::execute_functions_and_extract_list_results(
    const LispValuePtrVector &params,
    const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValuePtrVector result;

  for (const auto &param : params) {
    if (param->is_function()) {
      LispValue function_result(
          execution_context.execute(*param, context_param));

      // A function could return a list - we add the
      // list items separately to the result.
      if (function_result.is_list()) {
        for (const auto &item : function_result.list()) {
          result.push_back(item);
        }
      } else {
        result.push_back(std::make_shared<LispValue>(function_result));
      }
    } else {
      result.push_back(param);
    }
  }

  return result;
}
