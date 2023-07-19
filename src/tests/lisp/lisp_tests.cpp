#include "tools.h"
#include "lisp_tests.h"
#include "../../model/lisp/lisp_parser.h"

int run_lisp_tests_parsing1();

int run_lisp_tests() {
  TEST_ASSERT(run_lisp_tests_parsing1);

  return 0;
}

int run_lisp_tests_parsing1() {
  LispParser parser("(+ 22 4)");

  LispTokens tokens = parser.parse();

  TEST_ASSERT(tokens.size() == 7);

  return 0;
}
