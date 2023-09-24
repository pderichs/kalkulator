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


#ifndef LISP_EXECUTION_CONTEXT_FUNCALL_INCLUDED
#define LISP_EXECUTION_CONTEXT_FUNCALL_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_definition.h"
#include "lisp_function_execution_context.h"
#include "lisp_lambda_executor.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_ptr.h"

class LispExecutionContextFuncall : public LispFunctionExecutionContext {
public:
  LispExecutionContextFuncall() = default;
  ~LispExecutionContextFuncall() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    ensure_params(func);

    LispValuePtrVector params = extract_params_from_list(func);

    LispValuePtr lambda = params[0];
    if (!lambda->is_function()) {
      throw LispExecutionContextError(
          "Funcall: First parameter must be a lambda.");
    }

    LispValuePtr func_def_value =
        execution_context.execute(lambda->list(), context_param);
    if (!func_def_value->is_function_definition()) {
      throw LispExecutionContextError(
          "Funcall: Lambda did not return a closure.");
    }

    LispFunctionDefinition definition = func_def_value->function_definition();

    size_t lambda_param_count = params.size() - 1;
    if (lambda_param_count != definition.parameter_definitions.size()) {
      throw LispExecutionContextError("Funcall: Parameter count mismatch.");
    }

    LispValuePtrVector lambda_params = params;
    lambda_params.erase(lambda_params.begin());

    LispLambdaExecutor executor(definition, lambda_params);
    return executor.value(execution_context, context_param);
  }
};

#endif
