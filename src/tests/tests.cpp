#include "tests.h"
#include "lisp/lisp_tests.h"
#include "tools.h"

int run_tests() {
  RUN_TEST(run_lisp_tests);

  return 0;
}
