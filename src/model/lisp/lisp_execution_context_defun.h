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

#ifndef LISP_EXECUTION_CONTEXT_DEFUN_H
#define LISP_EXECUTION_CONTEXT_DEFUN_H

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextDefun : public LispFunctionExecutionContext {
public:
  LispExecutionContextDefun(LispExecutionContext *modifiable_execution_context)
      : _modifiable_execution_context(modifiable_execution_context) {}
  ~LispExecutionContextDefun() override = default;

  // Delete copy constructor and assignment operator
  LispExecutionContextDefun(const LispExecutionContextDefun &other) = delete;
  LispExecutionContextDefun &operator=(const LispExecutionContextDefun &other) = delete;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    std::ignore = execution_context;
    std::ignore = context_param;
    std::ignore = update_id;

    LispValuePtrVector params = extract_params_from_list(func);

    /*
     * (defun hello-world (name) "Simple function" (message-box "Hello, " name))
     */
    if (params.size() != 4) {
      return LispCommonValues::error_parameter_count();
    }

    if (!params[0]->is_identifier()) {
      return LispCommonValues::error_parameter();
    }

    if (!params[1]->is_list()) {
      return LispCommonValues::error_parameter();
    }

    if (!params[2]->is_string()) {
      return LispCommonValues::error_parameter();
    }

    if (!params[3]->is_list()) {
      return LispCommonValues::error_parameter();
    }

    LispFunctionDefinition def;
    def.name = params[0]->string();
    def.comment = params[2]->string();
    def.body = params[3]->list();

    for (const auto &param : params[1]->list()) {
      if (!param->is_identifier()) {
        return LispCommonValues::error_parameter();
      }

      def.parameter_definitions.push_back(param->string());
    }

    auto function_def = LispValueFactory::new_function_definition(def);
    _modifiable_execution_context->add_variable(def.name, function_def);

    return function_def;
  }

private:
  LispExecutionContext *_modifiable_execution_context;
};

#endif
