#include "lisp_tests.h"
#include "../../model/lisp/lisp_execution_context.h"
#include "../../model/lisp/lisp_function_execution_context.h"
#include "../../model/lisp/lisp_execution_context_error.h"
#include "../../model/lisp/lisp_function.h"
#include "../../model/lisp/lisp_parser.h"
#include "../../model/lisp/lisp_parser_error.h"
#include "../../model/lisp/lisp_value.h"
#include "../../model/lisp/lisp_value_parser.h"
#include "tools.h"

#include <any>
#include <memory>
#include <sstream>
#include <wx/wx.h>

int run_lisp_tests_parsing1();
int run_lisp_tests_parsing2();
int run_lisp_tests_parsing3();
int run_lisp_tests_parsing4();

int run_lisp_tests_expression1();
int run_lisp_tests_expression2();

int run_lisp_tests_wrong_form1();

int run_lisp_tests_executor1();
int run_lisp_tests_executor2();

int run_lisp_tests_custom_function1();

class TestLispFunctionExecutionContext: public LispFunctionExecutionContext {
public:
    virtual ~TestLispFunctionExecutionContext() = default;

    // Adds "Hello " to the front of the provided string
    virtual LispValue value(const LispFunction &func, const LispExecutionContext& execution_context) {
      ensure_params(func);

      if (func.param_count() != 1) {
        throw LispExecutionContextError("Unexpected parameter count");
      }

      auto param_opt = func.param_at(0);
      if (!param_opt) {
        throw LispExecutionContextError("Unable to get first parameter");
      }

      auto param = *param_opt;
      if (!param->is_string()) {
        throw LispExecutionContextError("Parameter must be of type string");
      }

      std::stringstream ss;
      ss << "Hello " << param->string() << "!";

      return LispValue(ss.str());
    }
};

int run_lisp_tests() {
  RUN_TEST(run_lisp_tests_parsing1);
  RUN_TEST(run_lisp_tests_parsing2);
  RUN_TEST(run_lisp_tests_parsing3);
  RUN_TEST(run_lisp_tests_parsing4);

  RUN_TEST(run_lisp_tests_expression1);
  RUN_TEST(run_lisp_tests_expression2);

  RUN_TEST(run_lisp_tests_wrong_form1);

  RUN_TEST(run_lisp_tests_executor1);
  RUN_TEST(run_lisp_tests_executor2);

  RUN_TEST(run_lisp_tests_custom_function1);

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

    LispValueParser parser(tokens);

    auto func = parser.next();
    TEST_ASSERT(func);

    LispValue val = *func;

    TEST_ASSERT(val.is_function());
    LispFunction expr = val.function();

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

    LispValueParser parser(tokens);

    auto func = parser.next();
    TEST_ASSERT(func);

    LispValue val = *func;
    TEST_ASSERT(val.is_function());

    LispFunction expr = val.function();

    TEST_ASSERT(expr.identifier() == "hello");
    // std::cerr << expr.param_count() << std::endl;
    TEST_ASSERT(expr.param_count() == 2);

    std::optional<LispValuePtr> optparam = expr.param_at(0);
    TEST_ASSERT(optparam);

    auto param = *optparam;
    TEST_ASSERT(param->is_function());
    TEST_ASSERT(param->function().identifier() == "-");

    optparam = expr.param_at(1);
    TEST_ASSERT(optparam);

    param = *optparam;
    TEST_ASSERT(param->is_number());
    TEST_ASSERT(param->number() == -484.32);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_wrong_form1() {
  StringVector cases = {
      "(", ")", "(343 1)", "(\"hello\" 1)", "123", "\"Hello\"",
  };

  for (auto c : cases) {
    bool exception = false;

    std::cerr << "Testing wrong form: " << c << std::endl;
    LispParser parser(c);

    try {
      LispTokens tokens = parser.parse();

      LispValueParser parser(tokens);
      exception = false;
    } catch (LispParserError &lpe) {
      // std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item:
      // \""
      //           << lpe.item() << "\")" << std::endl;

      // We expect an exception for the given form.
      exception = true;
    }

    if (!exception) {
      TEST_ASSERT(false);
    }
  }

  return 0;
}

int run_lisp_tests_executor1() {
  LispParser parser("(+ (- 100 2 30) -484.32)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto optvalue = parser.next();
    TEST_ASSERT(optvalue);

    auto value = *optvalue;

    LispExecutionContext executor;
    LispValue result = executor.execute(value);

    TEST_ASSERT(result == -416.32);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  } catch (LispExecutionContextError &lee) {
    std::cerr << "*** Caught lisp execution context error: " << lee.what()
              << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_executor2() {
  LispParser parser("(* (/ (* 100 2) (+ 25 25)) 8)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto optvalue = parser.next();
    TEST_ASSERT(optvalue);

    auto value = *optvalue;

    LispExecutionContext executor;
    LispValue result = executor.execute(value);

    TEST_ASSERT(result == 32.0);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  } catch (LispExecutionContextError &lee) {
    std::cerr << "*** Caught lisp execution context error: " << lee.what()
              << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_custom_function1() {
  LispParser parser("(say_hello_test \"Franzi\")");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto optvalue = parser.next();
    TEST_ASSERT(optvalue);

    auto value = *optvalue;

    LispExecutionContext executor;
    executor.add_function("say_hello_test", std::make_shared<TestLispFunctionExecutionContext>());
    LispValue result = executor.execute(value);

    TEST_ASSERT(result == "Hello Franzi!");
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  } catch (LispExecutionContextError &lee) {
    std::cerr << "*** Caught lisp execution context error: " << lee.what()
              << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}
