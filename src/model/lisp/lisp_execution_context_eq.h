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


#ifndef LISP_EXECUTION_CONTEXT_EQ_INCLUDED
#define LISP_EXECUTION_CONTEXT_EQ_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_value.h"

class LispExecutionContextEq : public LispFunctionExecutionContext {
public:
  explicit LispExecutionContextEq(bool extract_lists) : _extract_lists(
      extract_lists) {}
  ~LispExecutionContextEq() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    ensure_params(func);

    LispValuePtrVector params = extract_params_from_list(func);

    if (_extract_lists) {
      params = execute_functions_and_extract_list_results(
          params, execution_context, context_param, update_id);
    } else {
      params =
          execute_functions(params, execution_context, context_param, update_id);
    }

    LispValuePtr last;
    for (const auto &param : params) {
      if (last && !last->content_equals(*param)) {
        return LispCommonValues::false_value();
      }

      last = param;
    }

    return LispCommonValues::true_value();
  }

private:
  bool _extract_lists;
};

#endif
