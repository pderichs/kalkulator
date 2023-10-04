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

#include "gtest/gtest.h"

#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_execution_context_error.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"

#include "test_function_execution_context.h"

TEST(LispExecutionTests, ExecutorTest1) {
  LispParser parser("(+ (- 100 2 30) -484.32)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_EQ(*result, -416.32);
}

TEST(LispExecutionTests, ExecutorTest2) {
  LispParser parser("(* (/ (* 100 2) (+ 25 25)) 8)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_EQ(*result, 32.0);
}

TEST(LispExecutionTests, CustomFunctionTest1) {
  LispParser parser("(say_hello_test \"Franzi\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  executor.add_function("say_hello_test",
                        std::make_shared<TestLispFunctionExecutionContext>());
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_EQ(*result, "Hello Franzi!");
}