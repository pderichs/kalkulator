#include "lisp_execution_context.h"
#include "lisp_execution_context_addition.h"
#include "lisp_execution_context_and.h"
#include "lisp_execution_context_average.h"
#include "lisp_execution_context_cons.h"
#include "lisp_execution_context_division.h"
#include "lisp_execution_context_eq.h"
#include "lisp_execution_context_error.h"
#include "lisp_execution_context_first.h"
#include "lisp_execution_context_floor.h"
#include "lisp_execution_context_if.h"
#include "lisp_execution_context_join.h"
#include "lisp_execution_context_list.h"
#include "lisp_execution_context_multiplication.h"
#include "lisp_execution_context_not.h"
#include "lisp_execution_context_or.h"
#include "lisp_execution_context_rest.h"
#include "lisp_execution_context_subtraction.h"
#include "lisp_execution_context_xor.h"
#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include <memory>

LispExecutionContext::LispExecutionContext() {
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
}

LispValuePtr
LispExecutionContext::execute(const LispValuePtr &value,
                              const std::any &context_param = {}) const {
  if (!value->is_function()) {
    return value;
  }

  LispValuePtr execution_result =
      eval_function(value->function(), context_param);
  return execution_result;
}

LispValuePtr
LispExecutionContext::eval_function(const LispFunction &func,
                                    const std::any &context_param) const {
  const auto &execution_context_it = _functions.find(func.identifier());
  if (execution_context_it == _functions.end()) {
    throw LispExecutionContextError("Unknown function identifier");
  }

  const auto &function_context = execution_context_it->second;
  return function_context->value(func, *this, context_param);
}

void LispExecutionContext::add_function(
    const std::string &identifier, const LispFunctionExecutionContextPtr func) {
  if (!func) {
    throw LispExecutionContextError("Invalid function pointer");
  }

  const auto &execution_context_it = _functions.find(identifier);
  if (execution_context_it != _functions.end()) {
    throw LispExecutionContextError("Identifier is already assigned");
  }

  _functions[identifier] = func;
}
