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
#include "lisp_execution_context_progn.h"
#include "lisp_execution_context_rest.h"
#include "lisp_execution_context_subtraction.h"
#include "lisp_execution_context_xor.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include <memory>
#include <stdexcept>

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
  _functions["progn"] = std::make_shared<LispExecutionContextProgn>();
}

LispValuePtr
LispExecutionContext::execute(const LispValuePtr &value,
                              const std::any &context_param = {}) const {
  if (!value->is_function_or_possible_lambda()) {
    return value;
  }

  LispValuePtr execution_result = eval_function(value, context_param);
  return execution_result;
}

LispValuePtr
LispExecutionContext::eval_function(const LispValuePtr &func,
                                    const std::any &context_param) const {
  auto func_to_execute = func;

  if (func_to_execute->is_possible_lambda()) {
    // In order to get the function definition, we need to execute one extra
    // step here - the lambda needs to be executed with the given parameters
    // so it can create the "real" function execution body upfront.

    auto possible_lambda = func_to_execute->list()[0];

    // Call this function recursively and override function to execute
    // structure with lambda function result.
    func_to_execute = eval_function(possible_lambda, context_param);

    // We expect a function as a result here.
    if (!func_to_execute->is_function()) {
      throw LispExecutionContextError(
          "First function does not expand to executable context.");
    }
  }

  const auto &func_list = func_to_execute->list();

  const auto &execution_context_it = _functions.find(func_list.at(0)->string());
  if (execution_context_it == _functions.end()) {
    throw LispExecutionContextError("Unknown function identifier");
  }

  const auto &function_context = execution_context_it->second;
  return function_context->value(func_list, *this, context_param);
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
