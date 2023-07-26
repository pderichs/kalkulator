#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_value.h"

LispExecutionContext::LispExecutionContext(const LispValue &value) { _value = value; }

LispValue LispExecutionContext::execute() const {
  if (!_value.is_function()) {
    return _value;
  }

  LispValue execution_result = eval_function();
  return execution_result;
}

LispValue LispExecutionContext::eval_function() const {

  LispFunction func = _value.function();

  // TODO eval visitor

  if (func.identifier() == "+") {
    return add(func);
  } else if (func.identifier() == "-") {
    return subtract(func);
  } else if (func.identifier() == "*") {
    return multiply(func);
  } else if (func.identifier() == "/") {
    return divide(func);
  }

  // TODO: Add plugin based functions

  throw LispExecutionContextError("Unknown function identifier");
}

LispValue LispExecutionContext::add(const LispFunction &func) const {
  double val = 0;

  // for (auto param : func.params()) {
  //   val += param.value();
  // }

  return LispValue(val);
}

LispValue LispExecutionContext::subtract(const LispFunction &func) const {
  // double val = func.param_at(0).value();

  // int n = 0;
  // for (auto param : func.params()) {
  //   if (n == 0) {
  //     continue;
  //   }

  //   val -= param.value();
  //   n++;
  // }

  // return LispValue(val);

  return LispValue();
}

LispValue LispExecutionContext::multiply(const LispFunction &func) const {
  // double val = func.param_at(0).value();

  // int n = 0;
  // for (auto param : func.params()) {
  //   if (n == 0) {
  //     continue;
  //   }

  //   val *= param.value();
  //   n++;
  // }

  // return LispValue(val);

  return LispValue();
}

LispValue LispExecutionContext::divide(const LispFunction &func) const {
  // double val = func.param_at(0).value();

  // int n = 0;
  // for (auto param : func.params()) {
  //   if (n == 0) {
  //     continue;
  //   }

  //   double pv = param.value();

  //   if (pv == 0.0) {
  //     throw LispExecutorError("Division by zero");
  //   }

  //   val /= pv;
  //   n++;
  // }

  // return LispValue(val);

  return LispValue();
}
