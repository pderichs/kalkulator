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

#ifndef LISP_LAMBDA_EXECUTOR_INCLUDED
#define LISP_LAMBDA_EXECUTOR_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function_definition.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_ptr.h"
#include <cstddef>
#include <sstream>
#include <type_traits>

// TODO Maybe rename: We are not just executing lambdas here.
class LispLambdaExecutor {
public:
  LispLambdaExecutor(const LispFunctionDefinition &function_definition,
                     const LispValuePtrVector &params)
      : _definition(function_definition), _params(params) {}

  LispValuePtr value(const LispExecutionContext &execution_context,
                     const std::any &context_param, UpdateIdType update_id) {
    if (_definition.parameter_definitions.size() != _params.size()) {
      throw LispExecutionContextError(
          "Lambda Execution: Parameter count mismatch.");
    }

    // Match parameter names in lambda definition with actual
    // parameter values.
    std::map<std::string, LispValuePtr> named_params =
        match_params_names_with_values();

    // Replace parameter names with actual values for that call.
    // We are creating a copy of the definition list for that purpose.
    LispValuePtrVector def = _definition.body;

    LispValuePtr expanded_body = replace_names_with_values(def, named_params);

    return execution_context.execute(expanded_body->list(),
                                     context_param,
                                     update_id);
  }

  LispValuePtr replace_names_with_values(
      const LispValuePtrVector &list,
      std::map<std::string, LispValuePtr> named_params) const {

    LispValuePtrVector result;

    for (const auto &item : list) {
      if (item->is_identifier()) {
        auto it = named_params.find(item->string());
        if (it == named_params.end()) {
          result.push_back(item);
        } else {
          result.push_back(it->second);
        }
      } else if (item->is_list()) {
        // Recursion
        LispValuePtr sub_list =
            replace_names_with_values(item->list(), named_params);
        result.push_back(sub_list);
      } else {
        result.push_back(item);
      }
    }

    return LispValueFactory::new_list(result);
  }

  std::map<std::string, LispValuePtr> match_params_names_with_values() const {
    std::map<std::string, LispValuePtr> result;

    size_t n = 0;
    for (const auto &param_name : _definition.parameter_definitions) {
      const auto &param_value = _params[n];
      result[param_name] = param_value;
      n++;
    }

    return result;
  }

private:
  LispFunctionDefinition _definition;
  LispValuePtrVector _params;
};

#endif
