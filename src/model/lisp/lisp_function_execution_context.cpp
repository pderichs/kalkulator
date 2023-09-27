/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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

  if (value->is_function()) {
    result = execution_context.execute(value, context_param);
  } else if (value->is_number()) {
    result = value;
  } else if (value->is_none()) {
    result = LispValueFactory::new_double(0);
  } else {
    std::stringstream ss;
    ss << "Unable to perform this operation with this lisp value (expecting "
          "number): "
       << static_cast<int>(value->type());
    throw LispExecutionContextError(ss.str());
  }

  return result;
}

LispValuePtr LispFunctionExecutionContext::execute_if_required(
    const LispValuePtr &param, const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  if (param->is_function()) {
    LispValuePtr function_result(
        execution_context.execute(param, context_param));

    return function_result;
  }

  return param;
}

LispValuePtrVector LispFunctionExecutionContext::execute_functions(
    const LispValuePtrVector &params,
    const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValuePtrVector result;

  for (const auto &param : params) {
    result.push_back(execute_if_required(param,
                                         execution_context,
                                         context_param));
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

LispValuePtrVector LispFunctionExecutionContext::extract_params_from_list(
    const LispValuePtrVector &func) const {
  return {func.begin() + 1, func.end()};
}

LispValuePtrVector LispFunctionExecutionContext::extract_and_execute_params(
    const LispValuePtrVector &func,
    const LispExecutionContext &execution_context,
    const std::any &context_param) const {
  LispValuePtrVector result;

  result = extract_params_from_list(func);
  result = execute_functions_and_extract_list_results(result, execution_context,
                                                      context_param);

  return result;
}
