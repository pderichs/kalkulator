#ifndef LISP_EXECUTOR_INCLUDED
#define LISP_EXECUTOR_INCLUDED

#include <map>

#include "lisp_value.h"
#include "lisp_value_ptr.h"

class LispFunctionExecutionContext;

typedef std::shared_ptr<LispFunctionExecutionContext>
    LispFunctionExecutionContextPtr;

typedef std::map<std::string, LispFunctionExecutionContextPtr> LispFunctionMap;

class LispExecutionContext {
public:
  LispExecutionContext();
  LispValuePtr execute(const LispValuePtr &value,
                       const std::any &context_param) const;
  LispValuePtr execute(const LispValuePtrVector &func,
                       const std::any &context_param) const;
  void add_function(const std::string &identifier,
                    const LispFunctionExecutionContextPtr func);

private:
  LispValuePtr eval_function(const LispValuePtr &func,
                             const std::any &context_param = {}) const;

private:
  LispFunctionMap _functions;
};

#endif
