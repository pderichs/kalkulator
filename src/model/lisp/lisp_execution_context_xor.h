#ifndef LISP_EXECUTION_CONTEXT_XOR_INCLUDED
#define LISP_EXECUTION_CONTEXT_XOR_INCLUDED
#include <iostream>

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"

class LispExecutionContextXor : public LispFunctionExecutionContext {
public:
  LispExecutionContextXor() = default;
  virtual ~LispExecutionContextXor() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    LispValuePtrVector params = extract_params(func);

    if (params.size() != 2) {
      throw LispExecutionContextError("Xor: Expected 2 parameters");
    }

    params = execute_functions_and_extract_list_results(
        params, execution_context, context_param);

    const auto &param1 = params[0];
    const auto &param2 = params[1];

    if (!param1->is_boolean() || !param2->is_boolean()) {
      throw LispExecutionContextError(
          "Xor: Parameters must be of type boolean");
    }

    bool p1 = param1->boolean();
    bool p2 = param2->boolean();

    if (p1 != p2) {
      return LispValueFactory::new_bool(LISP_BOOL_TRUE);
    }

    return LispValueFactory::new_bool(LISP_BOOL_FALSE);
  }
};

#endif
