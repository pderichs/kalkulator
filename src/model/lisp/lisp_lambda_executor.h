#ifndef LISP_LAMBDA_EXECUTOR_INCLUDED
#define LISP_LAMBDA_EXECUTOR_INCLUDED

#include "lisp_execution_context_error.h"
#include "lisp_function_definition.h"
#include "lisp_function_execution_context.h"
#include "lisp_value.h"
#include "lisp_value_factory.h"
#include "lisp_value_ptr.h"
#include <cstddef>
#include <sstream>

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
    const auto &lambda_definition_parameters = _params;
    LispValuePtrVector lambda_factory_params = extract_params(func);

    if (lambda_definition_parameters.size() != lambda_factory_params.size()) {
      throw LispExecutionContextError("Lambda: Parameter count mismatch.");
    }

    LispValuePtrVector result;

    // Match parameter names in lambda definition with actual
    // parameter values.
    std::map<std::string, LispValuePtr> named_params;

    size_t n = 0;
    for (const auto &fac_param : lambda_factory_params) {
      const auto &def_param = lambda_definition_parameters[n];

      if (!def_param->is_identifier()) {
        throw LispExecutionContextError(
            "Lambda: All definition parameters must be identifiers.");
      }

      std::string param_name = def_param->string();
      named_params[param_name] = fac_param;

      n++;
    }

    if (!lambda_definition[2]->is_list()) {
      throw LispExecutionContextError(
          "Lambda: Definition must be of type list.");
    }

    const auto &def_body = lambda_definition[2]->list();
    LispValuePtr expanded_body =
        replace_names_with_values(def_body, named_params);

    return expanded_body;
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

private:
  LispValuePtrVector _params;
  LispFunctionDefinition _definition;
};

#endif
