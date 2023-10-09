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

#ifndef LISP_EXECUTOR_INCLUDED
#define LISP_EXECUTOR_INCLUDED

#include <map>

#include "lisp_value.h"
#include "lisp_value_ptr.h"
#include "lisp_common_values.h"
#include "update_id.h"

class LispFunctionExecutionContext;

typedef std::shared_ptr<LispFunctionExecutionContext>
    LispFunctionExecutionContextPtr;

typedef std::map<std::string, LispFunctionExecutionContextPtr> LispFunctionMap;

typedef std::map<std::string, LispValuePtr> LispScopeMap;

/**
 * Provides the context for a LispValue to be executed. This includes a
 * specific scope for variables and defined functions and functionality
 * to modify these.
 */
class LispExecutionContext {
public:
  LispExecutionContext();
  ~LispExecutionContext() = default;

  /**
   * Executes a lisp value. It returns the value if it is not a function.
   *
   * @param value Value to be executed or returned.
   * @param context_param Generic parameter which will be simple provided to each function execution handler
   * @param update_id Identifies the current update operation
   * @return The result of the operation. Might be the function result or simply the value itself.
   */
  LispValuePtr execute(const LispValuePtr &value,
                       const std::any &context_param,
                       UpdateIdType update_id) const;

  /**
   * Executes a lisp function.
   *
   * @param func The lisp list which represents the function to be executed
   * @param context_param Generic parameter which will be simple provided to each function execution handler
   * @param update_id Identifies the current update operation
   * @return The result of the operation.
   */
  LispValuePtr execute(const LispValuePtrVector &func,
                       const std::any &context_param,
                       UpdateIdType update_id) const;

  /**
   * Adds a function to this execution context.
   *
   * @param identifier Identifier for this function
   * @param func Native function execution context for this function
   */
  void add_function(const std::string &identifier,
                    const LispFunctionExecutionContextPtr &func,
                    bool override = false);

  /**
   * Adds a variable to the scope of this context.
   *
   * @param name Name of the variable
   * @param value Initial value of the variable, default is none
   */
  void add_variable(const std::string &name,
                    LispValuePtr value = LispCommonValues::none_value());

  /**
   * Extracts the variable names within the provided lisp list
   *
   * @param params Defined scope variables within this list will be expanded
   * @return List with expanded variable values
   */
  LispValuePtrVector extract_scope_variables(const LispValuePtrVector &params) const;

  /**
   * Returns the defined variable within this context
   *
   * @param name Name of the variable
   * @return The value of the variable
   */
  LispValuePtr scope_variable(const std::string &name) const;

private:
  LispScopeMap _scope;
  LispFunctionMap _functions;
};

#endif
