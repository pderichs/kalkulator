#include "lisp_runner.h"
#include "lisp_function.h"
#include "lisp_value.h"
#include "lisp_runtime_error.h"

LispRunner::LispRunner(const LispFunction& function): _function(function) {}

LispValue LispRunner::run() const {
  LispValue result;

  // TODO

  return result;
}
