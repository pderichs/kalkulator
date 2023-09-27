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
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"
#include "gtest/gtest.h"

TEST(LispFloorTests, FloorTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(floor 0)", LispValueFactory::new_double(std::floor(0))},
      {"(floor -5)", LispValueFactory::new_double(std::floor(-5))},
      {"(floor -15.377)", LispValueFactory::new_double(std::floor(-15.377))},
      {"(floor 18.847)", LispValueFactory::new_double(std::floor(18.847))},
  };

  return execute_lisp_tests(tests, "floor");
}

TEST(LispFloorTests, FloorDoesNotAcceptWrongParameterCountMore) {
  LispParser parser("(floor 1 \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispFloorTests, FloorDoesNotAcceptWrongParameterCountLess) {
  LispParser parser("(floor)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMCOUNTERR");
}

TEST(LispFloorTests, FloorDoesNotAcceptWrongParameterFormat) {
  LispParser parser("(floor \"Hello\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {});

  EXPECT_EQ(*result, "#PARAMERR");
}

