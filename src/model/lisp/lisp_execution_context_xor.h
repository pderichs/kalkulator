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

#ifndef LISP_EXECUTION_CONTEXT_XOR_INCLUDED
#define LISP_EXECUTION_CONTEXT_XOR_INCLUDED
#include <iostream>

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextXor : public LispFunctionExecutionContext {
public:
  LispExecutionContextXor() = default;
  ~LispExecutionContextXor() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    LispValuePtrVector params =
        extract_and_execute_params(func, execution_context, context_param);

    if (params.size() != 2) {
      throw LispExecutionContextError("Xor: Expected 2 parameters");
    }

    const auto &param1 = params[0];
    const auto &param2 = params[1];

    if (!param1->is_boolean() || !param2->is_boolean()) {
      throw LispExecutionContextError(
          "Xor: Parameters must be of type boolean");
    }

    bool p1 = param1->boolean();
    bool p2 = param2->boolean();

    if (p1 != p2) {
      return LispValueFactory::new_bool(LISP_BOOL_TRUE);
    }

    return LispValueFactory::new_bool(LISP_BOOL_FALSE);
  }
};

#endif
