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

#ifndef LISP_TEST_FUNCTION_EXECUTION_CONTEXT_H
#define LISP_TEST_FUNCTION_EXECUTION_CONTEXT_H

#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_function_execution_context.h"
#include "model/lisp/lisp_value_factory.h"

class TestLispFunctionExecutionContext : public LispFunctionExecutionContext {
public:
  // Adds "Hello " to the front of the provided string
  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    std::ignore = execution_context;
    std::ignore = context_param;
    std::ignore = update_id;

    ensure_params(func);

    LispValuePtrVector params = extract_params_from_list(func);

    if (params.size() != 1) {
      throw LispExecutionContextError("Unexpected parameter count");
    }

    auto param = params.at(0);
    if (!param->is_string()) {
      throw LispExecutionContextError("Parameter must be of type string");
    }

    std::stringstream ss;
    ss << "Hello " << param->string() << "!";

    return LispValueFactory::new_string(ss.str());
  }
};

#endif // LISP_TEST_FUNCTION_EXECUTION_CONTEXT_H
