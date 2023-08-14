#include "lisp_function_execution_context.h"
#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
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

LispValuePtrVector LispFunctionExecutionContext::execute_functions(
    const LispValuePtrVector &params,
    const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValuePtrVector result;

  for (const auto &param : params) {
    if (param->is_function()) {
      LispValue function_result(
          execution_context.execute(*param, context_param));

      result.push_back(std::make_shared<LispValue>(function_result));
    } else {
      result.push_back(param);
    }
  }

  return result;
}

LispValuePtrVector
LispFunctionExecutionContext::execute_functions_and_extract_list_results(
    const LispValuePtrVector &params,
    const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValuePtrVector result;

  auto exparams = execute_functions(params, execution_context, context_param);

  for (const auto &param : exparams) {
    if (param->is_list()) {
      for (const auto &item : param->list()) {
        result.push_back(item);
      }
    } else {
      result.push_back(param);
    }
  }

  return result;
}

LispValuePtr
LispFunctionExecutionContext::expect_parameter_at(const LispFunction &func,
                                                  size_t index) const {
  const auto &opt_param = func.param_at(index);

  if (!opt_param) {
    std::stringstream ss;
    ss << "Expected parameter at position " << index;
    throw LispExecutionContextError(ss.str());
  }

  const auto &value = *opt_param;

  if (!value) {
    std::stringstream ss;
    ss << "Expected parameter at position " << index;
    ss << " is invalid.";
    throw LispExecutionContextError(ss.str());
  }

  return *opt_param;
}
