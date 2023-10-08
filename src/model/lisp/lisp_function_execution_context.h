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

/**
 * Base class for all native lisp function implementations.
 */
class LispFunctionExecutionContext {
public:
  LispFunctionExecutionContext() = default;
  virtual ~LispFunctionExecutionContext() = default;

  /**
   * Executes the function and returns the result.
   *
   * @param func Function to be executed
   * @param execution_context Execution context to be used for running the function
   * @param context_param Generic parameter for each execution
   * @param update_id Identifies the current update operation
   * @return Result of the operation
   */
  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) = 0;

protected:
  /**
   * Executes possible function calls within provided list
   *
   * @param params List to be executed
   * @param execution_context Execution context used for this operation
   * @param context_param Generic parameter for each execution
   * @param update_id Identifies the current update operation
   * @return List containing function execution results
   */
  LispValuePtrVector
  execute_functions(const LispValuePtrVector &params,
                    const LispExecutionContext &execution_context,
                    const std::any &context_param,
                    UpdateIdType update_id) const;

  /**
   * Executes all functions and expands the lists
   *
   * @param params Parameters to operate on
   * @param execution_context Execution context used for this operation
   * @param context_param Generic parameter for each execution
   * @param update_id Identifies the current update operation
   * @return List which contains the function results and extracted list items
   */
  LispValuePtrVector execute_functions_and_extract_list_results(
      const LispValuePtrVector &params,
      const LispExecutionContext &execution_context,
      const std::any &context_param, UpdateIdType update_id) const;

  /**
   * Ensures the provided function call contains parameters.
   * @param func List to be checked
   */
  void ensure_params(const LispValuePtrVector &func) const;

  /**
   * Ensures the provided value is representing a number. Also converts e.g.
   * none to 0.
   *
   * @param value Value to be checked
   * @param execution_context Execution context used for this operation
   * @param context_param Generic parameter for each execution
   * @param update_id Identifies the current update operation
   * @return Value in number representation
   */
  LispValuePtr expect_number(const LispValuePtr &value,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) const;

  /**
   * Returns the parameters of a function
   *
   * @param func List representing a function call
   * @return Parameter list of the function call
   */
  LispValuePtrVector
  extract_params_from_list(const LispValuePtrVector &func) const;

  /**
   * Helper function which combines the functionality of extract_params_from_list
   * and execute_functions_and_extract_list_results.
   *
   * @param func Function to operate on
   * @param execution_context Execution context used for this operation
   * @param context_param Generic parameter for each execution
   * @param update_id Identifies the current update operation
   * @return List containing function results and extracted items
   */
  LispValuePtrVector
  extract_and_execute_params(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param,
                             UpdateIdType update_id) const;
};

#endif
