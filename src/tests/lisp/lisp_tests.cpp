#include "lisp_tests.h"
#include "../../model/lisp/lisp_parser.h"

int run_lisp_tests_parsing1();

int run_lisp_tests() {
  // TODO

  return 0;
}

int run_lisp_tests_parsing1() {
  LispParser parser("(+ 2 4)");

  LispTokens tokens = parser.parse();

  return 0;
}
