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
#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"
#include "gtest/gtest.h"

void execute_lisp_tests(const std::map<std::string, LispValuePtr> &tests,
                       const std::string &test_name) {
  for (const auto &it : tests) {
    std::string test = it.first;
    const auto &expected_result = it.second;

    std::cerr << test_name << ": test for " << test << std::endl;

    LispParser parser(test);

    LispTokens tokens;
    EXPECT_NO_THROW(tokens = parser.parse());

    LispValueParser value_parser(tokens);

    auto value = value_parser.next();
    EXPECT_TRUE(value);

    LispExecutionContext executor;
    LispValuePtr result = executor.execute(value, {});

    EXPECT_EQ(*result, *expected_result);
  }
}