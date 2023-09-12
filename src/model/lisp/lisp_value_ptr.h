#ifndef LISP_VALUE_PTR_INCLUDED
#define LISP_VALUE_PTR_INCLUDED

#include <vector>
#include <memory>

class LispValue;
typedef std::shared_ptr<LispValue> LispValuePtr;
typedef std::vector<LispValuePtr> LispValuePtrVector;

#endif
