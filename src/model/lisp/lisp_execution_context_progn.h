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

#ifndef EXECUTION_CONTEXT_PROGN_INCLUDED
#define EXECUTION_CONTEXT_PROGN_INCLUDED

#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextProgn : public LispFunctionExecutionContext {
public:
  LispExecutionContextProgn() = default;
  ~LispExecutionContextProgn() override = default;

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

    // Only the last result is relevant for progn
    return params[params.size() - 1];
  }
};

#endif
