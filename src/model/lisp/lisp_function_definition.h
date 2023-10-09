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

#ifndef LISP_CLOSURE_DEFINITION_INCLUDED
#define LISP_CLOSURE_DEFINITION_INCLUDED

#include <map>
#include <string>

#include "lisp_value_ptr.h"
#include "tools.h"

/**
 * Used for function definitions and closure definitions.
 * If used for closure definitions, name is empty.
 */
struct LispFunctionDefinition {
  std::string name;
  std::string comment;
  StringVector parameter_definitions;
  LispValuePtrVector body;

  LispFunctionDefinition()
      : name(), comment(), parameter_definitions(), body() {}
};

#endif
