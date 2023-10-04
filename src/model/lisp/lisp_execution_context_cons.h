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

#ifndef LISP_EXECUTION_CONTEXT_CONS_INCLUDED
#define LISP_EXECUTION_CONTEXT_CONS_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include <tuple>

class LispExecutionContextCons : public LispFunctionExecutionContext {
public:
  LispExecutionContextCons() = default;
  ~LispExecutionContextCons() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    std::ignore = execution_context;
    std::ignore = context_param;
    std::ignore = update_id;

    LispValuePtrVector params = extract_params_from_list(func);

    if (params.size() != 2) {
      throw LispExecutionContextError("cons expects two parameters");
    }

    LispValuePtrVector result;

    for (const auto &param : params) {
      result.push_back(param);
    }

    return LispValueFactory::new_list(result);
  }
};

#endif
