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

#ifndef LISP_EXECUTION_CONTEXT_SET_VAR_H
#define LISP_EXECUTION_CONTEXT_SET_VAR_H

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextSetVar : public LispFunctionExecutionContext {
public:
  LispExecutionContextSetVar(LispExecutionContext *modifiable_execution_context)
      : _modifiable_execution_context(modifiable_execution_context) {}
  ~LispExecutionContextSetVar() override = default;

  // Delete copy constructor and assignment operator
  LispExecutionContextSetVar(const LispExecutionContextSetVar &other) = delete;
  LispExecutionContextSetVar &operator=(const LispExecutionContextSetVar &other) = delete;

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

    if (!params[0]->is_identifier()) {
      return LispCommonValues::error_parameter();
    }

    _modifiable_execution_context->add_variable(params[0]->string(), params[1]);

    // Return the assigned value
    return params[1];
  }

private:
  LispExecutionContext *_modifiable_execution_context;
};

#endif
