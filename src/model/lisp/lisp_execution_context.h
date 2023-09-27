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

class LispFunctionExecutionContext;

typedef std::shared_ptr<LispFunctionExecutionContext>
    LispFunctionExecutionContextPtr;

typedef std::map<std::string, LispFunctionExecutionContextPtr> LispFunctionMap;

typedef std::map<std::string, LispValuePtr> LispScopeMap;

class LispExecutionContext {
public:
  LispExecutionContext();
  LispValuePtr execute(const LispValuePtr &value,
                       const std::any &context_param) const;
  LispValuePtr execute(const LispValuePtrVector &func,
                       const std::any &context_param) const;
  void add_function(const std::string &identifier,
                    const LispFunctionExecutionContextPtr &func,
                    bool override = false);

  void add_variable(const std::string &name,
                    LispValuePtr value = LispCommonValues::none_value());

  LispValuePtrVector extract_scope_variables(const LispValuePtrVector &params) const;

  LispValuePtr scope_variable(const std::string& name) const;

private:
  LispScopeMap _scope;
  LispFunctionMap _functions;
};

#endif
