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

#ifndef LISP_ERROR_VALUES_H
#define LISP_ERROR_VALUES_H

#include "model/lisp/lisp_value.h"

/**
 * Provides some often used default LispValue instances
 */
class LispCommonValues {
public:
  /**
   * @return Error string returned for a wrong parameter count
   */
  static LispValuePtr error_parameter_count();

  /**
   * @return Error string for a wrong parameter format or unexpected parameter
   */
  static LispValuePtr error_parameter();

  /**
   * @return String indicating a circular reference error
   */
  static LispValuePtr error_circular_ref();

  /**
   * @return LispValue which represents a true value
   */
  static LispValuePtr true_value();

  /**
   * @return LispValue which represents a false value
   */
  static LispValuePtr false_value();

  /**
   * @return LispValue which represents a none value
   */
  static LispValuePtr none_value();

private:
  static LispValuePtr _error_parameter_count;
  static LispValuePtr _error_parameter;
  static LispValuePtr _error_circular_ref;
  static LispValuePtr _true_value;
  static LispValuePtr _false_value;
  static LispValuePtr _none_value;
};

#endif
