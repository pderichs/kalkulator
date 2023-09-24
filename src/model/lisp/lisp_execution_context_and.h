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

#ifndef LISP_EXECUTION_CONTEXT_AND_INCLUDED
#define LISP_EXECUTION_CONTEXT_AND_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextAnd : public LispFunctionExecutionContext {
public:
  LispExecutionContextAnd() = default;
  ~LispExecutionContextAnd() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    ensure_params(func);

    LispValuePtrVector params = extract_params_from_list(func);

    for (const auto &param : params) {
      LispValuePtr actual_value =
          execute_if_required(param, execution_context, context_param);
      if (!actual_value->is_truthy()) {
        return LispValueFactory::new_bool(LISP_BOOL_FALSE);
      }
    }

    return LispValueFactory::new_bool(LISP_BOOL_TRUE);
  }
};

#endif
