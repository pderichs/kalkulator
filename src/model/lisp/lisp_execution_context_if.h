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
  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    LispValuePtrVector params = extract_params_from_list(func);

    if (params.size() < 2 || params.size() > 3) {
      throw LispExecutionContextError("Unexpected parameter count for if");
    }

    LispValuePtr condition = params[0];

    if (condition->is_function()) {
      condition =
          execution_context.execute(condition, context_param, update_id);
    }

    LispValuePtr result;

    if (!condition->is_boolean()) {
      std::cerr << "CONDITION IS NOT BOOL" << std::endl;
      return LispCommonValues::error_parameter();
    }

    if (condition->boolean()) {
      result = execution_context.execute(params[1], context_param, update_id);
    } else {
      if (params.size() == 3) {
        result = execution_context.execute(params[2], context_param, update_id);
      } else {
        result = LispCommonValues::none_value();
      }
    }

    return result;
  }
};

#endif
