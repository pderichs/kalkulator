#include "lisp_tests.h"
#include "../../model/lisp/lisp_function.h"
#include "../../model/lisp/lisp_parser.h"
#include "../../model/lisp/lisp_parser_error.h"
#include "../../model/lisp/lisp_function_error.h"
#include "../../model/lisp/lisp_value.h"
#include "tools.h"

#include <any>
#include <wx/wx.h>

int run_lisp_tests_parsing1();
int run_lisp_tests_parsing2();
int run_lisp_tests_parsing3();
int run_lisp_tests_parsing4();

int run_lisp_tests_expression1();
int run_lisp_tests_expression2();

int run_lisp_tests() {
  RUN_TEST(run_lisp_tests_parsing1);
  RUN_TEST(run_lisp_tests_parsing2);
  RUN_TEST(run_lisp_tests_parsing3);
  RUN_TEST(run_lisp_tests_parsing4);
  RUN_TEST(run_lisp_tests_expression1);
  RUN_TEST(run_lisp_tests_expression2);

  return 0;
}

int run_lisp_tests_parsing1() {
  LispParser parser("(+ 22 4)");

  try {
    LispTokens tokens = parser.parse();

    // std::cerr << "Tokens size: " << tokens.size() << std::endl;

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

    // std::cerr << "Tokens size: " << tokens.size() << std::endl;

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

int run_lisp_tests_parsing3() {
  LispParser parser("(hello \"8282\" -484.32)");

  try {
    LispTokens tokens = parser.parse();

    // std::cerr << "Tokens size: " << tokens.size() << std::endl;

    TEST_ASSERT(tokens[0].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[1].id == IDENTIFIER);
    std::string s = std::any_cast<std::string>(tokens[1].content);
    TEST_ASSERT(s == "hello");
    TEST_ASSERT(tokens[2].id == SPACE);
    TEST_ASSERT(tokens[3].id == STRING);
    s = std::any_cast<std::string>(tokens[3].content);
    std::cerr << s << std::endl;
    TEST_ASSERT(s == "8282");
    TEST_ASSERT(tokens[4].id == SPACE);
    TEST_ASSERT(tokens[5].id == NUMBER);
    double d = std::any_cast<double>(tokens[5].content);
    TEST_ASSERT(d == -484.32);
    TEST_ASSERT(tokens[6].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens.size() == 7);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_parsing4() {
  LispParser parser("(funktion1 \"(* 5.344 22)\" \")\")");

  try {
    LispTokens tokens = parser.parse();

    // std::cerr << "Tokens size: " << tokens.size() << std::endl;

    TEST_ASSERT(tokens[0].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[1].id == IDENTIFIER);
    std::string s = std::any_cast<std::string>(tokens[1].content);
    TEST_ASSERT(s == "funktion1");
    TEST_ASSERT(tokens[2].id == SPACE);
    TEST_ASSERT(tokens[3].id == STRING);
    s = std::any_cast<std::string>(tokens[3].content);
    std::cerr << s << std::endl;
    TEST_ASSERT(s == "(* 5.344 22)");
    TEST_ASSERT(tokens[4].id == SPACE);
    TEST_ASSERT(tokens[5].id == STRING);
    s = std::any_cast<std::string>(tokens[5].content);
    TEST_ASSERT(s == ")");
    TEST_ASSERT(tokens[6].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens.size() == 7);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_expression1() {
  LispParser parser("(hello \"8282\" -484.32)");

  try {
    LispTokens tokens = parser.parse();

    LispFunction expr(tokens);

    double d;
    std::string s;

    TEST_ASSERT(expr.identifier() == "hello");
    TEST_ASSERT(expr.param_count() == 2);

    {
      const auto &value = expr.param_at(0);
      TEST_ASSERT(value);
      TEST_ASSERT((*value)->is_string());
      s = (*value)->string();
      TEST_ASSERT(s == "8282");
    }

    {
      const auto &value = expr.param_at(1);
      TEST_ASSERT(value);
      TEST_ASSERT((*value)->is_number());
      d = (*value)->number();
      TEST_ASSERT(d == -484.32);
    }
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  } catch (std::bad_any_cast &bac) {
    std::cerr << "*** Caught bad any cast: " << bac.what() << std::endl;
    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_expression2() {
  LispParser parser("(hello (- 100 2 30) -484.32)");

  try {
    LispTokens tokens = parser.parse();

    LispFunction expr(tokens);

    // TEST_ASSERT(expr.identifier() == "hello");
    // TEST_ASSERT(expr.param_count() == 2);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  } catch (LispFunctionError &lfe) {
    std::cerr << "*** Caught lisp function error: " << lfe.what() << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

// TODO test error cases / exceptions
