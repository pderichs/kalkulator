#include "lisp_function_execution_context.h"
#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include <memory>
#include <sstream>

void LispFunctionExecutionContext::ensure_params(
    const LispValuePtrVector &func) const {
  if (func.size() <= 1) {
    throw LispExecutionContextError("Unexpected: No params given");
  }
}

LispValuePtr LispFunctionExecutionContext::expect_number(
    const LispValuePtr &value, const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValuePtr result;

  if (value->is_function_or_possible_lambda()) {
    result = execution_context.execute(value, context_param);
  } else if (value->is_number()) {
    result = value;
  } else if (value->is_none()) {
    result = LispValueFactory::new_double(0);
  } else {
    std::stringstream ss;
    ss << "Unable to perform this operation with this lisp value (expecting "
          "number) "
       << static_cast<int>(value->type());
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
    if (param->is_function_or_possible_lambda()) {
      LispValuePtr function_result(
          execution_context.execute(param, context_param));

      result.push_back(function_result);
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

LispValuePtr LispFunctionExecutionContext::expect_parameter_at(
    const LispValuePtrVector &params, size_t index) const {
  try {
    const auto &param = params.at(index);
    return param;
  } catch (std::out_of_range const &exc) {
    std::stringstream ss;
    ss << "Expected parameter at position " << index;
    throw LispExecutionContextError(ss.str());
  }
}

LispValuePtrVector LispFunctionExecutionContext::extract_params(
    const LispValuePtrVector &func) const {
  return LispValuePtrVector(func.begin() + 1, func.end());
}
