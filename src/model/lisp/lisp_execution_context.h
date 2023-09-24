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

class LispFunctionExecutionContext;

typedef std::shared_ptr<LispFunctionExecutionContext>
    LispFunctionExecutionContextPtr;

typedef std::map<std::string, LispFunctionExecutionContextPtr> LispFunctionMap;

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

private:
  LispValuePtr eval_function(const LispValuePtr &func,
                             const std::any &context_param = {}) const;

private:
  LispFunctionMap _functions;
};

#endif
