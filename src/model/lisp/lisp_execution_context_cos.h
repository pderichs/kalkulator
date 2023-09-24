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

#ifndef LISP_EXECUTION_CONTEXT_COS_H
#define LISP_EXECUTION_CONTEXT_COS_H

#include "lisp_execution_context_error.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextCos : public LispFunctionExecutionContext {
public:
  LispExecutionContextCos() = default;
  ~LispExecutionContextCos() override = default;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param) override {
    LispValuePtrVector params =
        extract_and_execute_params(func, execution_context, context_param);

    if (params.size() != 1) {
      return LispValueFactory::new_string("#PARAMCOUNTERR");
    }

    if (!params[0]->is_number()) {
      return LispValueFactory::new_string("#PARAMERR");
    }

    LispValue::DoubleType result = std::cos(params[0]->to_double());
    return LispValueFactory::new_double(result);
  }
};


#endif // LISP_EXECUTION_CONTEXT_COS_H