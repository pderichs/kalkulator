/**
 * Kalkulator - a simple and small spread sheet app.
 * Copyright (C) 2023  pderichs (derichs@posteo.de)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "lisp_test_tools.h"
#include "mock_function_context.h"
#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(LispIfTests, IfTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {R"((if (= 3 3) "OK" "Not ok"))", LispValueFactory::new_string("OK")},
      {"(if true 1 0)", LispValueFactory::new_integer(1)},
      {"(if false 0 42)", LispValueFactory::new_integer(42)},
      {R"((if (= 4 3) "Not ok" "GOOD!"))", LispValueFactory::new_string("GOOD!")},
      {R"((if (= 4 3) "Not ok"))", LispValueFactory::new_none()},
  };

  return execute_lisp_tests(tests, "if");
}

TEST(LispIfTests, IfTestWithLambda1) {
  LispParser parser("(if (= 4 4 5) 666 (lambda (x y) (+ x y)))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);
  EXPECT_TRUE(value->is_function());

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_function_definition());
  EXPECT_EQ(result->function_definition().name, "");
}

TEST(LispIfTests, IfTestWithLambdaFalseCaseIsNotExecutedWhenConditionIsTrue) {
  LispParser parser("(if (= 4 4 4) 666 (lambda (x y) (+ x y)))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);
  EXPECT_TRUE(value->is_function());

  std::shared_ptr<MockFunctionContext> fct =
      std::make_shared<MockFunctionContext>();
  LispExecutionContext executor;
  executor.add_function("lambda", fct, true);

  // We want to make sure the lambda is not executed
  EXPECT_CALL(*fct, value).Times(0);

  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_integer());
  EXPECT_EQ(result->explicit_integer_value(), 666);
}

TEST(LispIfTests, IfTestWithLambdaTrueCaseIsNotExecutedWhenConditionIsFalse) {
  LispParser parser("(if (= 4 5 4) (lambda (x y) (+ x y)) 666)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);
  EXPECT_TRUE(value->is_function());

  std::shared_ptr<MockFunctionContext> fct =
      std::make_shared<MockFunctionContext>();
  LispExecutionContext executor;
  executor.add_function("lambda", fct, true);

  // We want to make sure the lambda is not executed
  EXPECT_CALL(*fct, value).Times(0);

  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_integer());
  EXPECT_EQ(result->explicit_integer_value(), 666);
}
