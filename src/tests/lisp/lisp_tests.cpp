#include "lisp_tests.h"
#include "../../model/lisp/lisp_parser.h"
#include "../../model/lisp/lisp_parser_error.h"
#include "tools.h"
#include <any>

#include <wx/wx.h>

int run_lisp_tests_parsing1();
int run_lisp_tests_parsing2();

int run_lisp_tests() {
  RUN_TEST(run_lisp_tests_parsing1);
  RUN_TEST(run_lisp_tests_parsing2);

  return 0;
}

int run_lisp_tests_parsing1() {
  LispParser parser("(+ 22 4)");

  try {
    LispTokens tokens = parser.parse();

    //std::cerr << "Tokens size: " << tokens.size() << std::endl;

    TEST_ASSERT(tokens[0].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[1].id == IDENTIFIER);
    TEST_ASSERT(tokens[2].id == SPACE);
    TEST_ASSERT(tokens[3].id == NUMBER);
    TEST_ASSERT(tokens[4].id == SPACE);
    TEST_ASSERT(tokens[5].id == NUMBER);
    TEST_ASSERT(tokens[6].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens.size() == 7);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_parsing2() {
  LispParser parser("(some-function \"Hello \\\"World\\\"\" 22 4)");

  try {
    LispTokens tokens = parser.parse();

    //std::cerr << "Tokens size: " << tokens.size() << std::endl;

    TEST_ASSERT(tokens[0].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[1].id == IDENTIFIER);
    std::string s = std::any_cast<std::string>(tokens[1].content);
    TEST_ASSERT(s == "some-function");
    TEST_ASSERT(tokens[2].id == SPACE);
    TEST_ASSERT(tokens[3].id == STRING);
    s = std::any_cast<std::string>(tokens[3].content);
    std::cerr << s << std::endl;
    TEST_ASSERT(s == "Hello \\\"World\\\"");
    TEST_ASSERT(tokens[4].id == SPACE);
    TEST_ASSERT(tokens[5].id == NUMBER);
    double d = std::any_cast<double>(tokens[5].content);
    TEST_ASSERT(d == 22.0);
    TEST_ASSERT(tokens[6].id == SPACE);
    TEST_ASSERT(tokens[7].id == NUMBER);
    d = std::any_cast<double>(tokens[7].content);
    TEST_ASSERT(d == 4.0);
    TEST_ASSERT(tokens[8].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens.size() == 9);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}
