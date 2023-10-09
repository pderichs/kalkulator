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

#ifndef VALUE_CONVERTER_INCLUDED
#define VALUE_CONVERTER_INCLUDED

#include "lisp_execution_context.h"
#include "update_id.h"

/**
 * Converts strings to LispValues and vice versa.
 *
 * NOTE Function execution is separately callable since we
 * want to be able to cache function results.
 */
class ValueConverter {
public:
  /**
   * Assigns the function context to be used with the converter.
   * This should be set at the start of each application using this
   * class.
   *
   * @param context The execution context which should be used for functions.
   */
  static void set_execution_context(LispExecutionContext *context);

  /**
   * Converts a string to a lisp value.
   *
   * @param s String which should be converted
   * @return A LispValue representing the contents of the string.
   */
  static LispValuePtr to_lisp_value(const std::string &s);

  /**
   * Creates a string representation of the given lisp value.
   *
   * @param value Value to be represented as a string
   * @param context_param Context param used for the execution context
   * @param update_id Id of this operation
   * @return A string representing the value
   */
  static std::string to_string(const LispValuePtr &value,
                               const std::any &context_param,
                               UpdateIdType update_id);

  /**
   * Executes a function within the context of the previously assigned function
   * context.
   *
   * @param function LispValue containing a function
   * @param context_param Context param used for the execution context
   * @param update_id Id of this operation
   * @return The result of the function
   */
  static LispValuePtr execute_function(const LispValuePtr &function,
                                       const std::any &context_param,
                                       UpdateIdType update_id);

private:
  static LispExecutionContext *execution_context;
};

#endif
