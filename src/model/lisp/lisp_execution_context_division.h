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

#ifndef LISP_EXECUTION_CONTEXT_DIVISION_INCLUDED
#define LISP_EXECUTION_CONTEXT_DIVISION_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextDivision : public LispFunctionExecutionContext {
public:
  LispExecutionContextDivision() = default;
  ~LispExecutionContextDivision() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    ensure_params(func);

    LispValuePtrVector params =
        extract_and_execute_params(func,
                                   execution_context,
                                   context_param,
                                   update_id);

    LispValue::DoubleType result;

    const auto &first_param = params[0];

    LispValuePtr base_value(
        expect_number(first_param, execution_context, context_param, update_id));

    // First parameter of subtraction is base value
    result = base_value->to_double();

    // Skip first param
    for (size_t n = 1; n < params.size(); n++) {
      const auto &param = params[n];
      LispValuePtr value(
          expect_number(param, execution_context, context_param, update_id));

      if (value->to_double() == 0.0) {
        throw LispExecutionContextError("Division by zero");
      }

      result /= value->to_double();
    }

    return LispValueFactory::new_double(result);
  }
};

#endif
