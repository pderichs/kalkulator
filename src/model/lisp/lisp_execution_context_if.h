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


#ifndef LISP_EXECUTION_CONTEXT_IF_INCLUDED
#define LISP_EXECUTION_CONTEXT_IF_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include <cstddef>
#include <memory>

class LispExecutionContextIf : public LispFunctionExecutionContext {
public:
  LispExecutionContextIf() = default;
  ~LispExecutionContextIf() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    LispValuePtrVector params = extract_params(func);

    if (params.size() < 2 || params.size() > 3) {
      throw LispExecutionContextError("Unexpected parameter count for if");
    }

    LispValuePtr condition = expect_parameter_at(params, 0);

    if (condition->is_function()) {
      condition = execution_context.execute(condition, context_param);
    }

    LispValuePtr result;

    if (condition->is_truthy()) {
      result = execute_result_param(func, 1, execution_context, context_param);
    } else {
      result = execute_result_param(func, 2, execution_context, context_param);
    }

    return result;
  }

  [[nodiscard]] LispValuePtr
  execute_result_param(const LispValuePtrVector &func, size_t index,
                       const LispExecutionContext &execution_context,
                       const std::any &context_param) const {
    LispValuePtr result;

    LispValuePtrVector params = extract_params(func);

    const auto &result_param = expect_parameter_at(params, index);

    if (result_param->is_function()) {
      result = execution_context.execute(result_param, context_param);
    } else {
      result = result_param;
    }

    return result;
  }
};

#endif
