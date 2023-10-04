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
#include "model/lisp/lisp_value_factory.h"
#include "model/lisp/lisp_value_parser.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "lisp/lisp_common_values.h"

TEST(LispAndTests, AndTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(and (= 1 0))", LispCommonValues::false_value()},
      {"(and (= 1 0) (= 0 1))", LispCommonValues::false_value()},
      {"(and (= 1 1))", LispCommonValues::true_value()},
      {"(and (= 1 1) (= 0 1))", LispCommonValues::false_value()},
      {"(and (= 1 0) (= 1 1))", LispCommonValues::false_value()},
      {"(and (= 1 1) (= 1 1))", LispCommonValues::true_value()},
  };

  return execute_lisp_tests(tests, "and");
}

TEST(LispAndTests, AndDoesNotExecuteOtherConditionIfFirstIsFalseAlready) {
  LispParser parser("(and (= 4 4 6) (lambda (x y) (+ x y)))");

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

  EXPECT_TRUE(result->is_boolean());
  EXPECT_FALSE(result->boolean());
}
