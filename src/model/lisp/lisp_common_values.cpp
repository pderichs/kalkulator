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

#include "lisp_common_values.h"
#include "lisp_value_factory.h"

LispValuePtr LispCommonValues::_error_parameter_count =
    LispValueFactory::new_string("#PARAMCOUNTERR");
LispValuePtr
    LispCommonValues::_error_parameter = LispValueFactory::new_string("#PARAMERR");
LispValuePtr LispCommonValues::_error_generic =
    LispValueFactory::new_string("#ERR");
LispValuePtr LispCommonValues::_error_circular_ref =
    LispValueFactory::new_string("#CIRCULARREF");
LispValuePtr LispCommonValues::_true_value =
    LispValueFactory::new_bool(LISP_BOOL_TRUE);
LispValuePtr LispCommonValues::_false_value =
    LispValueFactory::new_bool(LISP_BOOL_FALSE);
LispValuePtr LispCommonValues::_nil_value =
    LispValueFactory::new_none();

LispValuePtr LispCommonValues::error_parameter_count() {
  return _error_parameter_count;
}

LispValuePtr LispCommonValues::error_parameter() {
  return _error_parameter;
}
LispValuePtr LispCommonValues::error_generic() {
  return _error_generic;
}
LispValuePtr LispCommonValues::error_circular_ref() {
  return _error_circular_ref;
}
LispValuePtr LispCommonValues::true_value() {
  return _true_value;
}
LispValuePtr LispCommonValues::false_value() {
  return _false_value;
}
LispValuePtr LispCommonValues::nil_value() {
  return _nil_value;
}
