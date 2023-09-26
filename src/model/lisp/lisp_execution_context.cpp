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

#include "lisp_execution_context.h"
#include "lisp_execution_context_addition.h"
#include "lisp_execution_context_and.h"
#include "lisp_execution_context_average.h"
#include "lisp_execution_context_cons.h"
#include "lisp_execution_context_cos.h"
#include "lisp_execution_context_division.h"
#include "lisp_execution_context_eq.h"
#include "lisp_execution_context_error.h"
#include "lisp_execution_context_first.h"
#include "lisp_execution_context_floor.h"
#include "lisp_execution_context_funcall.h"
#include "lisp_execution_context_greater_than.h"
#include "lisp_execution_context_if.h"
#include "lisp_execution_context_join.h"
#include "lisp_execution_context_lambda.h"
#include "lisp_execution_context_list.h"
#include "lisp_execution_context_lower_than.h"
#include "lisp_execution_context_multiplication.h"
#include "lisp_execution_context_not.h"
#include "lisp_execution_context_or.h"
#include "lisp_execution_context_progn.h"
#include "lisp_execution_context_rest.h"
#include "lisp_execution_context_sin.h"
#include "lisp_execution_context_subtraction.h"
#include "lisp_execution_context_tan.h"
#include "lisp_execution_context_xor.h"
#include "lisp_value.h"
#include "lisp_value_ptr.h"
#include "lisp_execution_context_abs.h"
#include "lisp_execution_context_ceil.h"
#include "lisp_execution_context_sqrt.h"
#include "lisp_execution_context_pow.h"
#include "lisp_execution_context_log.h"
#include "lisp_execution_context_isnone.h"
#include <memory>
#include <sstream>
#include <stdexcept>

LispExecutionContext::LispExecutionContext() : _scope(), _functions() {
  // Prepare default scope
  _scope["nil"] = LispCommonValues::nil_value();
  _scope["true"] = LispCommonValues::true_value();
  _scope["false"] = LispCommonValues::false_value();

  // Prepare functions
  _functions["+"] = std::make_shared<LispExecutionContextAddition>();
  _functions["-"] = std::make_shared<LispExecutionContextSubtraction>();
  _functions["*"] = std::make_shared<LispExecutionContextMultiplication>();
  _functions["/"] = std::make_shared<LispExecutionContextDivision>();
  _functions["list"] = std::make_shared<LispExecutionContextList>();
  _functions["first"] = std::make_shared<LispExecutionContextFirst>();
  _functions["rest"] = std::make_shared<LispExecutionContextRest>();
  _functions["join"] = std::make_shared<LispExecutionContextJoin>();
  _functions["cons"] = std::make_shared<LispExecutionContextCons>();
  _functions["if"] = std::make_shared<LispExecutionContextIf>();
  _functions["eq"] = std::make_shared<LispExecutionContextEq>(false);
  _functions["xeq"] = std::make_shared<LispExecutionContextEq>(
      true); // read as: "extracted equal"
  _functions["="] = _functions["xeq"];
  _functions["not"] = std::make_shared<LispExecutionContextNot>();
  _functions["or"] = std::make_shared<LispExecutionContextOr>();
  _functions["xor"] = std::make_shared<LispExecutionContextXor>();
  _functions["and"] = std::make_shared<LispExecutionContextAnd>();
  _functions["avg"] = std::make_shared<LispExecutionContextAvg>();
  _functions["floor"] = std::make_shared<LispExecutionContextFloor>();
  _functions["progn"] = std::make_shared<LispExecutionContextProgn>();
  _functions["lambda"] = std::make_shared<LispExecutionContextLambda>();
  _functions["funcall"] = std::make_shared<LispExecutionContextFuncall>();
  _functions[">"] = std::make_shared<LispExecutionContextGreaterThan>();
  _functions["<"] = std::make_shared<LispExecutionContextLowerThan>();
  _functions["tan"] = std::make_shared<LispExecutionContextTan>();
  _functions["cos"] = std::make_shared<LispExecutionContextCos>();
  _functions["sin"] = std::make_shared<LispExecutionContextSin>();
  _functions["abs"] = std::make_shared<LispExecutionContextAbs>();
  _functions["ceil"] = std::make_shared<LispExecutionContextCeil>();
  _functions["sqrt"] = std::make_shared<LispExecutionContextSqrt>();
  _functions["pow"] = std::make_shared<LispExecutionContextPow>();
  _functions["log"] = std::make_shared<LispExecutionContextLog>();
  _functions["is-none"] = std::make_shared<LispExecutionContextIsNone>();
}

LispValuePtr
LispExecutionContext::execute(const LispValuePtr &value,
                              const std::any &context_param = {}) const {
  if (!value->is_function()) {
    return value;
  }

  LispValuePtr execution_result = execute(value->list(), context_param);
  return execution_result;
}

LispValuePtr
LispExecutionContext::execute(const LispValuePtrVector &func,
                              const std::any &context_param) const {
  std::string identifier = func.at(0)->string();
  const auto &execution_context_it = _functions.find(identifier);
  if (execution_context_it == _functions.end()) {
    std::stringstream ss;
    ss << "Unknown function identifier: \"";
    ss << identifier;
    ss << "\"";
    throw LispExecutionContextError(ss.str());
  }

  const auto &function_context = execution_context_it->second;
  return function_context->value(func, *this, context_param);
}

void LispExecutionContext::add_function(
    const std::string &identifier, const LispFunctionExecutionContextPtr &func,
    bool override) {
  if (!func) {
    throw LispExecutionContextError("Invalid function pointer");
  }

  const auto &execution_context_it = _functions.find(identifier);
  if (!override && execution_context_it != _functions.end()) {
    throw LispExecutionContextError("Identifier is already assigned");
  }

  _functions[identifier] = func;
}

LispValuePtrVector LispExecutionContext::extract_scope_variables(const LispValuePtrVector &params) const {
  LispValuePtrVector result;

  for (const auto &param : params) {
    if (param->is_identifier()) {
      std::string id = param->string();

      auto it = _scope.find(id);
      if (it != _scope.end()) {
        result.push_back(it->second);
        continue;
      }
    }

    result.push_back(param);
  }

  return result;
}
