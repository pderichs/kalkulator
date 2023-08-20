#ifndef LISP_EXECUTOR_INCLUDED
#define LISP_EXECUTOR_INCLUDED

#include <map>

#include "lisp_function.h"
#include "lisp_value.h"

class LispFunctionExecutionContext;

typedef std::shared_ptr<LispFunctionExecutionContext>
    LispFunctionExecutionContextPtr;

typedef std::map<std::string, LispFunctionExecutionContextPtr> LispFunctionMap;

class LispExecutionContext {
private:
  LispFunctionMap _functions;

public:
  LispExecutionContext();
  LispValuePtr execute(const LispValuePtr &value,
                       const std::any &context_param) const;
  void add_function(const std::string &identifier,
                    const LispFunctionExecutionContextPtr func);

private:
  LispValuePtr eval_function(const LispFunction &func,
                          const std::any &context_param = {}) const;
};

#endif
