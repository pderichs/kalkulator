#ifndef LISP_EXECUTION_CONTEXT_LAMBDA_INCLUDED
#define LISP_EXECUTION_CONTEXT_LAMBDA_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_definition.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_ptr.h"

class LispExecutionContextLambda : public LispFunctionExecutionContext {
public:
  LispExecutionContextLambda() = default;
  virtual ~LispExecutionContextLambda() = default;

  virtual LispValuePtr value(const LispValuePtrVector &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    std::ignore = execution_context;
    std::ignore = context_param;

    LispValuePtrVector params = extract_params(func);

    if (params.size() != 2) {
      throw LispExecutionContextError("Lambda: Unexpected parameter count.");
    }

    LispValuePtr param_defs = params[0];
    if (!param_defs->is_list()) {
      throw LispExecutionContextError(
          "Lambda: Second parameter must be parameter definition list.");
    }

    LispValuePtr body = params[1];
    if (!body->is_function()) {
      throw LispExecutionContextError("Lambda: Body has unexpected type.");
    }

    LispFunctionDefinition definition;

    for (const auto &param : param_defs->list()) {
      if (!param->is_identifier()) {
        throw LispExecutionContextError(
            "Lambda: All parameters must be identifiers.");
      }

      definition.parameter_definitions.push_back(param->string());
    }

    definition.body = body->list();

    return LispValueFactory::new_function_definition(definition);
  }
};

#endif
