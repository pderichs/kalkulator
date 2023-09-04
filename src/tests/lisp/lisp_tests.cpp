#include "lisp_tests.h"
#include "../../model/lisp/lisp_execution_context.h"
#include "../../model/lisp/lisp_execution_context_error.h"
#include "../../model/lisp/lisp_function.h"
#include "../../model/lisp/lisp_function_execution_context.h"
#include "../../model/lisp/lisp_parser.h"
#include "../../model/lisp/lisp_parser_error.h"
#include "../../model/lisp/lisp_value.h"
#include "../../model/lisp/lisp_value_factory.h"
#include "../../model/lisp/lisp_value_parser.h"
#include "tools.h"

#include <any>
#include <memory>
#include <sstream>
#include <tuple>
#include <wx/wx.h>

int run_lisp_tests(const std::map<std::string, LispValuePtr> tests,
                   const std::string &test_name);

int run_lisp_tests_parsing1();
int run_lisp_tests_parsing2();
int run_lisp_tests_parsing3();
int run_lisp_tests_parsing4();

int run_lisp_tests_expression1();
int run_lisp_tests_expression2();
int run_lisp_tests_expression_with_identifier1();

int run_lisp_tests_wrong_form1();

int run_lisp_tests_executor1();
int run_lisp_tests_executor2();

int run_lisp_tests_custom_function1();

int run_lisp_tests_list1();

int run_lisp_tests_addition();
int run_lisp_tests_addition_with_list1();
int run_lisp_tests_subtraction();
int run_lisp_tests_subtraction_with_list1();
int run_lisp_tests_multiplication();
int run_lisp_tests_multiplication_with_list1();
int run_lisp_tests_division();
int run_lisp_tests_division_with_list1();

int run_lisp_tests_first1();

int run_lisp_tests_rest1();

int run_lisp_tests_join1();

int run_lisp_tests_cons1();

int run_lisp_tests_if1();
int run_lisp_tests_if2();

int run_lisp_tests_eq1();

int run_lisp_tests_xeq1();

int run_lisp_tests_not1();

int run_lisp_tests_or1();

int run_lisp_tests_and1();

int run_lisp_tests_xor1();

int run_lisp_tests_avg1();

int run_lisp_tests_progn();

int run_lisp_tests_lambda_parsing();

class TestLispFunctionExecutionContext : public LispFunctionExecutionContext {
public:
  virtual ~TestLispFunctionExecutionContext() = default;

  // Adds "Hello " to the front of the provided string
  virtual LispValuePtr value(const LispFunction &func,
                             const LispExecutionContext &execution_context,
                             const std::any &context_param) {
    std::ignore = execution_context;
    std::ignore = context_param;

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

    return LispValueFactory::new_string(ss.str());
  }
};

int run_lisp_tests() {
  RUN_TEST(run_lisp_tests_parsing1);
  RUN_TEST(run_lisp_tests_parsing2);
  RUN_TEST(run_lisp_tests_parsing3);
  RUN_TEST(run_lisp_tests_parsing4);

  RUN_TEST(run_lisp_tests_expression1);
  RUN_TEST(run_lisp_tests_expression2);

  RUN_TEST(run_lisp_tests_expression_with_identifier1);

  RUN_TEST(run_lisp_tests_wrong_form1);

  RUN_TEST(run_lisp_tests_executor1);
  RUN_TEST(run_lisp_tests_executor2);

  RUN_TEST(run_lisp_tests_custom_function1);

  RUN_TEST(run_lisp_tests_list1);

  RUN_TEST(run_lisp_tests_addition);
  RUN_TEST(run_lisp_tests_addition_with_list1);
  RUN_TEST(run_lisp_tests_subtraction);
  RUN_TEST(run_lisp_tests_subtraction_with_list1);
  RUN_TEST(run_lisp_tests_multiplication);
  RUN_TEST(run_lisp_tests_multiplication_with_list1);
  RUN_TEST(run_lisp_tests_division);
  RUN_TEST(run_lisp_tests_division_with_list1);

  RUN_TEST(run_lisp_tests_first1);

  RUN_TEST(run_lisp_tests_rest1);

  RUN_TEST(run_lisp_tests_join1);

  RUN_TEST(run_lisp_tests_cons1);

  RUN_TEST(run_lisp_tests_if1);
  RUN_TEST(run_lisp_tests_if2);

  RUN_TEST(run_lisp_tests_eq1);

  RUN_TEST(run_lisp_tests_xeq1);

  RUN_TEST(run_lisp_tests_not1);

  RUN_TEST(run_lisp_tests_or1);

  RUN_TEST(run_lisp_tests_and1);

  RUN_TEST(run_lisp_tests_xor1);

  RUN_TEST(run_lisp_tests_avg1);

  RUN_TEST(run_lisp_tests_progn);

  RUN_TEST(run_lisp_tests_lambda_parsing);

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
    TEST_ASSERT(tokens[3].id == INTEGER);
    TEST_ASSERT(tokens[4].id == SPACE);
    TEST_ASSERT(tokens[5].id == INTEGER);
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
    TEST_ASSERT(tokens[5].id == INTEGER);
    LispValue::IntegerType d =
        std::any_cast<LispValue::IntegerType>(tokens[5].content);
    TEST_ASSERT(d == 22.0);
    TEST_ASSERT(tokens[6].id == SPACE);
    TEST_ASSERT(tokens[7].id == INTEGER);
    d = std::any_cast<LispValue::IntegerType>(tokens[7].content);
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
    TEST_ASSERT(tokens[5].id == DOUBLE);
    LispValue::DoubleType d =
        std::any_cast<LispValue::DoubleType>(tokens[5].content);
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

    LispValue::DoubleType d;
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
      d = (*value)->to_double();
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
    TEST_ASSERT(param->to_double() == -484.32);
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

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == -416.32);
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

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 32.0);
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

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    executor.add_function("say_hello_test",
                          std::make_shared<TestLispFunctionExecutionContext>());
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == "Hello Franzi!");
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

int run_lisp_tests_list1() {
  LispParser parser("(list 42 \"Hallo\" (+ 3 900))");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(result->is_list());

    LispValuePtrVector lst = result->list();
    TEST_ASSERT(lst.size() == 3);

    LispValuePtr item;
    item = lst[0];
    TEST_ASSERT(item->is_number());

    item = lst[1];
    TEST_ASSERT(item->is_string());

    item = lst[2];
    TEST_ASSERT(item->is_function());
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

int run_lisp_tests_expression_with_identifier1() {
  LispParser parser("(hello 33 some-variable \"Hello world\")");

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
    TEST_ASSERT(expr.param_count() == 3);

    std::optional<LispValuePtr> optparam = expr.param_at(0);
    TEST_ASSERT(optparam);

    auto param = *optparam;
    TEST_ASSERT(param->is_number());
    TEST_ASSERT((*param) == 33);

    optparam = expr.param_at(1);
    TEST_ASSERT(optparam);

    param = *optparam;
    TEST_ASSERT(param->is_identifier());
    TEST_ASSERT((*param) == "some-variable");

    optparam = expr.param_at(2);
    TEST_ASSERT(optparam);

    param = *optparam;
    TEST_ASSERT(param->is_string());
    TEST_ASSERT((*param) == "Hello world");
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_addition() {
  LispParser parser("(+ 19 -32 6)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == -7);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_subtraction() {
  LispParser parser("(- 19 -32 6)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 45);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_multiplication() {
  LispParser parser("(* 19 -32 6)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == -3648);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_division() {
  LispParser parser("(/ 2000 2 10)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 100);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_addition_with_list1() {
  LispParser parser("(+ 10 (list 2000 20) 6)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 2036);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_subtraction_with_list1() {
  LispParser parser("(- 10000 (list 2000 20) 6)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 7974);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_multiplication_with_list1() {
  LispParser parser("(* 20 (list 2000 20) 6)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 4800000);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_division_with_list1() {
  LispParser parser("(/ 10000 (list 20 10) 5)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 10);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_first1() {
  LispParser parser("(first 24000 (list 20 10) 5)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(*result == 24000);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_rest1() {
  LispParser parser("(rest 24000 (list 20 10) 5)");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(result->is_list());

    const auto &lst = result->list();

    TEST_ASSERT(*lst[0] == 20);
    TEST_ASSERT(*lst[1] == 10);
    TEST_ASSERT(*lst[2] == 5);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_join1() {
  LispParser parser("(join (list 39 345 \"Hello\" 5858.22 1) 42 (list 0 0 0))");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(result->is_list());

    const auto &lst = result->list();

    TEST_ASSERT(*lst[0] == 39);
    TEST_ASSERT(*lst[1] == 345);
    TEST_ASSERT(*lst[2] == "Hello");
    TEST_ASSERT(*lst[3] == 5858.22);
    TEST_ASSERT(*lst[4] == 1);
    TEST_ASSERT(*lst[5] == 42);
    TEST_ASSERT(*lst[6] == 0);
    TEST_ASSERT(*lst[7] == 0);
    TEST_ASSERT(*lst[8] == 0);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_cons1() {
  LispParser parser("(cons 42 \"Hello\")");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(result->is_list());

    const auto &lst = result->list();

    TEST_ASSERT(*lst[0] == 42);
    TEST_ASSERT(*lst[1] == "Hello");
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_if1() {
  LispParser parser("(if (= 3 3) \"OK\" \"Not ok\")");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(result->is_string());

    TEST_ASSERT(*result == "OK");
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_if2() {
  LispParser parser("(if (= 4 3) \"Not ok\" \"GOOD!\")");

  try {
    LispTokens tokens = parser.parse();

    LispValueParser parser(tokens);

    auto value = parser.next();
    TEST_ASSERT(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    TEST_ASSERT(result->is_string());

    TEST_ASSERT(*result == "GOOD!");
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}

int run_lisp_tests_eq1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
      {"(eq 1 1)", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq \"Hase\" 1)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq \"Hase\" \"Bär\")", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq \"Hase\" \"Hase\")", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq (list 54 2 66 9.326) (list 54 2 66 9.326))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq (list 54 2 66 9.326) (list 54 3 66 9.326))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq (+ 54 2 66 9.3265) (+ 54 2 66 9.3265))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(eq (list 6 6 6) (list 6 6 6) 6)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(eq (list 6 6 6) (list 6 6 6) 5)", LispValueFactory::new_bool(LISP_BOOL_FALSE)}};
  // clang-format on

  return run_lisp_tests(tests, "eq");
}

int run_lisp_tests_xeq1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
      {"(xeq 1 1)", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(xeq \"Hase\" 1)", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(xeq \"Hase\" \"Bär\")", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(xeq \"Hase\" \"Hase\")", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(xeq (list 54 2 66 9.326) (list 54 2 66 9.326))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
      {"(xeq (+ 54 2 66 9.3265) (+ 54 2 66 9.3265))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(xeq (list 6 6 6) (list 6 6 6) 6)", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
      {"(xeq (list 6 6 6) (list 6 6 6) 5)", LispValueFactory::new_bool(LISP_BOOL_FALSE)}};
  // clang-format on

  return run_lisp_tests(tests, "xeq");
}

int run_lisp_tests_not1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
     {"(not (= 1 0))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(not (= 1 0) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(not (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(not (= 1 1) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
  };
  // clang-format on

  return run_lisp_tests(tests, "not");
}

int run_lisp_tests_or1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
     {"(or (= 1 0))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(or (= 1 0) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(or (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(or (= 1 1) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(or (= 1 0) (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(or (= 1 1) (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
  };
  // clang-format on

  return run_lisp_tests(tests, "or");
}

int run_lisp_tests_and1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
     {"(and (= 1 0))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(and (= 1 0) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(and (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(and (= 1 1) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(and (= 1 0) (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(and (= 1 1) (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
  };
  // clang-format on

  return run_lisp_tests(tests, "and");
}

int run_lisp_tests_xor1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
     {"(xor (= 1 0) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
     {"(xor (= 1 1) (= 0 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(xor (= 1 0) (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_TRUE)},
     {"(xor (= 1 1) (= 1 1))", LispValueFactory::new_bool(LISP_BOOL_FALSE)},
  };
  // clang-format on

  return run_lisp_tests(tests, "xor");
}

int run_lisp_tests_avg1() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
     {"(avg 2 2 2)", LispValueFactory::new_double(2)},
     {"(avg 42)", LispValueFactory::new_double(42)},
  };
  // clang-format on

  return run_lisp_tests(tests, "avg");
}

int run_lisp_tests(const std::map<std::string, LispValuePtr> tests,
                   const std::string &test_name) {
  for (const auto &it : tests) {
    std::string test = it.first;
    const auto &expected_result = it.second;

    std::cerr << test_name << ": test for " << test << std::endl;

    LispParser parser(test);

    try {
      LispTokens tokens = parser.parse();

      LispValueParser parser(tokens);

      auto value = parser.next();
      TEST_ASSERT(value);

      LispExecutionContext executor;
      LispValuePtr result = executor.execute(value, {});

      TEST_ASSERT(*result == *expected_result);
    } catch (LispParserError &lpe) {
      std::cerr << "*** Caught lisp parser error: " << lpe.what()
                << " (item: \"" << lpe.item() << "\")" << std::endl;

      TEST_ASSERT(false);
    } catch (LispExecutionContextError &lece) {
      std::cerr << "*** Caught lisp parser error: " << lece.what() << std::endl;

      TEST_ASSERT(false);
    }
  }

  return 0;
}

int run_lisp_tests_progn() {
  // clang-format off
  std::map<std::string, LispValuePtr> tests = {
     {"(progn 2 2 9)", LispValueFactory::new_integer(9)},
     {"(progn 42)", LispValueFactory::new_integer(42)},
     {"(progn 1 1 \"Hello!\")", LispValueFactory::new_string("Hello!")},
  };
  // clang-format on

  return run_lisp_tests(tests, "progn");
}

int run_lisp_tests_lambda_parsing() {
  LispParser parser("((lambda (x) (+ x 10)) 22)");

  try {
    LispTokens tokens = parser.parse();

    TEST_ASSERT(tokens[0].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[1].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[2].id == IDENTIFIER);
    TEST_ASSERT(tokens[3].id == SPACE);
    TEST_ASSERT(tokens[4].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[5].id == IDENTIFIER);
    TEST_ASSERT(tokens[6].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens[7].id == SPACE);
    TEST_ASSERT(tokens[8].id == OPEN_BRACKET);
    TEST_ASSERT(tokens[9].id == IDENTIFIER);
    TEST_ASSERT(tokens[10].id == SPACE);
    TEST_ASSERT(tokens[11].id == IDENTIFIER);
    TEST_ASSERT(tokens[12].id == SPACE);
    TEST_ASSERT(tokens[13].id == INTEGER);
    TEST_ASSERT(tokens[14].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens[15].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens[16].id == SPACE);
    TEST_ASSERT(tokens[17].id == INTEGER);
    TEST_ASSERT(tokens[18].id == CLOSE_BRACKET);
    TEST_ASSERT(tokens.size() == 19);
  } catch (LispParserError &lpe) {
    std::cerr << "*** Caught lisp parser error: " << lpe.what() << " (item: \""
              << lpe.item() << "\")" << std::endl;

    TEST_ASSERT(false);
  }

  return 0;
}
