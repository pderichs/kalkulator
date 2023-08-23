#ifndef LISP_EXECUTION_CONTEXT_FLOOR_INCLUDED
#define LISP_EXECUTION_CONTEXT_FLOOR_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function.h"
#include "lisp_function_execution_context.h"
#include "lisp_value_factory.h"
#include <cmath>

class LispExecutionContextFloor : public LispFunctionExecutionContext {
public:
  LispExecutionContextFloor() = default;
  virtual ~LispExecutionContextFloor() = default;

  virtual LispValuePtr value(const LispFunction &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    if (func.param_count() != 1) {
      throw LispExecutionContextError("floor: parameter expected");
    }

    LispValuePtrVector params = execute_functions_and_extract_list_results(
        func.params(), execution_context, context_param);

    return LispValueFactory::new_double(std::floor(params[0]->to_double()));
  }
};

#endif
