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


#ifndef LISP_EXECUTION_CONTEXT_LAMBDA_INCLUDED
#define LISP_EXECUTION_CONTEXT_LAMBDA_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_definition.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_ptr.h"

class LispExecutionContextLambda : public LispFunctionExecutionContext {
public:
  LispExecutionContextLambda() = default;
  ~LispExecutionContextLambda() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    std::ignore = execution_context;
    std::ignore = context_param;

    LispValuePtrVector params = extract_params_from_list(func);

    if (params.size() != 2) {
      throw LispExecutionContextError("Lambda: Unexpected parameter count.");
    }

    LispValuePtr param_defs = params[0];
    if (!param_defs->is_list()) {
      throw LispExecutionContextError(
          "Lambda: Second parameter must be parameter definition list.");
    }

    LispValuePtr body = params[1];
    if (!body->is_function()) {
      throw LispExecutionContextError("Lambda: Body has unexpected type.");
    }

    LispFunctionDefinition definition;

    for (const auto &param : param_defs->list()) {
      if (!param->is_identifier()) {
        throw LispExecutionContextError(
            "Lambda: All parameters must be identifiers.");
      }

      definition.parameter_definitions.push_back(param->string());
    }

    definition.body = body->list();

    return LispValueFactory::new_function_definition(definition);
  }
};

#endif
