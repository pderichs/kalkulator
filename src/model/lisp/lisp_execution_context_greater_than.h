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

#ifndef LISP_EXECUTION_CONTEXT_GREATER_THAN_H
#define LISP_EXECUTION_CONTEXT_GREATER_THAN_H

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextGreaterThan : public LispFunctionExecutionContext {
public:
  LispExecutionContextGreaterThan() = default;
  ~LispExecutionContextGreaterThan() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    LispValuePtrVector params =
        extract_and_execute_params(func,
                                   execution_context,
                                   context_param,
                                   update_id);

    if (params.size() != 2) {
      return LispCommonValues::error_parameter_count();
    }

    if (!params[0]->is_number() || !params[1]->is_number()) {
      return LispCommonValues::error_parameter();
    }

    if (params[0]->to_double() > params[1]->to_double()) {
      return LispCommonValues::true_value();
    }

    return LispCommonValues::false_value();
  }
};

#endif // LISP_EXECUTION_CONTEXT_GREATER_THAN_H
