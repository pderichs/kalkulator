#ifndef LISP_LAMBDA_EXECUTOR_INCLUDED
#define LISP_LAMBDA_EXECUTOR_INCLUDED

#include "lisp_execution_context.h"
#include "lisp_execution_context_error.h"
#include "lisp_function_definition.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_ptr.h"
#include <cstddef>
#include <sstream>
#include <type_traits>

// TODO Maybe rename: We are not just executing lambdas here.
class LispLambdaExecutor {
public:
  LispLambdaExecutor(const LispFunctionDefinition &function_definition,
                     const LispValuePtrVector &params) {
    _definition = function_definition;
    _params = params;
  }
  virtual ~LispLambdaExecutor() = default;

  virtual LispValuePtr value(const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    if (_definition.parameter_definitions.size() != _params.size()) {
      throw LispExecutionContextError(
          "Lambda Execution: Parameter count mismatch.");
    }

    // Match parameter names in lambda definition with actual
    // parameter values.
    std::map<std::string, LispValuePtr> named_params =
        match_params_names_with_values();

    // Replace parameter names with actual values for that call.
    // We are creating a copy of the definition list for that purpose.
    LispValuePtrVector def = _definition.body;

    LispValuePtr expanded_body = replace_names_with_values(def, named_params);

    return execute_actual_body(def, execution_context, context_param);
  }

  LispValuePtr
  execute_actual_body(const LispValuePtrVector& def,
                      const LispExecutionContext &execution_context,
                      const std::any &context_param) const {
    return execution_context.execute(def, context_param);
  }

  LispValuePtr replace_names_with_values(
      const LispValuePtrVector &list,
      std::map<std::string, LispValuePtr> named_params) const {

    LispValuePtrVector result;

    for (const auto &item : list) {
      if (item->is_identifier()) {
        auto it = named_params.find(item->string());
        if (it == named_params.end()) {
          result.push_back(item);
        } else {
          result.push_back(it->second);
        }
      } else if (item->is_list()) {
        // Recursion
        LispValuePtr sub_list =
            replace_names_with_values(item->list(), named_params);
        result.push_back(sub_list);
      } else {
        result.push_back(item);
      }
    }

    return LispValueFactory::new_list(result);
  }

  std::map<std::string, LispValuePtr> match_params_names_with_values() const {
    std::map<std::string, LispValuePtr> result;

    size_t n = 0;
    for (const auto &param_name : _definition.parameter_definitions) {
      const auto &param_value = _params[n];
      result[param_name] = param_value;
      n++;
    }

    return result;
  }

private:
  LispValuePtrVector _params;
  LispFunctionDefinition _definition;
};

#endif
