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

#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_function_execution_context.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"
#include "gtest/gtest.h"

TEST(LispJoinTests, JoinTest1) {
  LispParser parser("(join (list 39 345 \"Hello\" 5858.22 1) 42 (list 0 0 0))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  LispExecutionContext executor;
  LispValuePtr result = executor.execute(value, {}, 0);

  EXPECT_TRUE(result->is_list());

  const auto &lst = result->list();

  EXPECT_EQ(*lst[0], 39);
  EXPECT_EQ(*lst[1], 345);
  EXPECT_EQ(*lst[2], "Hello");
  EXPECT_EQ(*lst[3], 5858.22);
  EXPECT_EQ(*lst[4], 1);
  EXPECT_EQ(*lst[5], 42);
  EXPECT_EQ(*lst[6], 0);
  EXPECT_EQ(*lst[7], 0);
  EXPECT_EQ(*lst[8], 0);
}