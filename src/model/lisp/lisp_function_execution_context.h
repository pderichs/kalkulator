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

#ifndef LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED
#define LISP_FUNCTION_EXECUTION_CONTEXT_INCLUDED

#include <cstddef>
#include <memory>

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_value.h"

class LispFunctionExecutionContext {
public:
  LispFunctionExecutionContext() = default;
  virtual ~LispFunctionExecutionContext() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) = 0;

protected:
  LispValuePtrVector
  execute_functions(const LispValuePtrVector &params,
                    const LispExecutionContext &execution_context,
                    const std::any &context_param,
                    UpdateIdType update_id) const;
  LispValuePtrVector execute_functions_and_extract_list_results(
      const LispValuePtrVector &params,
      const LispExecutionContext &execution_context,
      const std::any &context_param, UpdateIdType update_id) const;

  void ensure_params(const LispValuePtrVector &func) const;
  LispValuePtr expect_number(const LispValuePtr &value,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) const;

  LispValuePtrVector
  extract_params_from_list(const LispValuePtrVector &func) const;

  LispValuePtrVector
  extract_and_execute_params(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) const;
};

#endif
