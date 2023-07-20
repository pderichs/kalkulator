#include "tools.h"
#include "lisp_tests.h"
#include "../../model/lisp/lisp_parser.h"
#include "../../model/lisp/lisp_parser_error.h"

#include <wx/wx.h>

int run_lisp_tests_parsing1();

int run_lisp_tests() {
  RUN_TEST(run_lisp_tests_parsing1);

  return 0;
}

int run_lisp_tests_parsing1() {
  LispParser parser("(+ 22 4)");

  try {
  LispTokens tokens = parser.parse();

  TEST_ASSERT(tokens.size() == 7);
  } catch (LispParserError& lpe) {
    wxPrintf("*** Caught lisp parser error: %s\n", lpe.what());
    TEST_ASSERT(false);
  }

  return 0;
}
