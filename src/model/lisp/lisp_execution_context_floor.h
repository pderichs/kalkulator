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

#ifndef LISP_EXECUTION_CONTEXT_FLOOR_INCLUDED
#define LISP_EXECUTION_CONTEXT_FLOOR_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include <cmath>

class LispExecutionContextFloor : public LispFunctionExecutionContext {
public:
  LispExecutionContextFloor() = default;
  ~LispExecutionContextFloor() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    LispValuePtrVector params =
        extract_and_execute_params(func, execution_context, context_param);

    if (params.size() != 1) {
      throw LispExecutionContextError("floor: unexpected parameter count");
    }

    return LispValueFactory::new_double(std::floor(params[0]->to_double()));
  }
};

#endif
