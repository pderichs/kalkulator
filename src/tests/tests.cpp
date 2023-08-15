#include "tests.h"
#include "lisp/lisp_tests.h"
#include "lisp/lisp_value_tests.h"
#include "table/table_lisp_function_tests.h"
#include "tools.h"

int run_tests() {
  RUN_TEST(run_lisp_tests);
  RUN_TEST(run_table_lisp_function_tests);
  RUN_TEST(run_lisp_value_tests);

  return 0;
}
