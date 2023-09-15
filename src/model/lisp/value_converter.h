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

class ValueConverter {
public:
  static void set_execution_context(LispExecutionContext *context);

  static LispValuePtr to_lisp_value(const std::string &s);
  static std::string to_string(const LispValuePtr &value,
                               const std::any &context_param);

private:
  static LispExecutionContext *execution_context;
};

#endif
