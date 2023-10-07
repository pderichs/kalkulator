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
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_factory.h"
#include "model/lisp/lisp_value_parser.h"
#include "gtest/gtest.h"

TEST(LispLambdaTests, LambdaParsingTest1) {
  LispParser parser("((lambda (x) (+ x 10)) 22)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  EXPECT_EQ(tokens[0].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[1].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[2].id, IDENTIFIER);
  EXPECT_EQ(tokens[3].id, SPACE);
  EXPECT_EQ(tokens[4].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[5].id, IDENTIFIER);
  EXPECT_EQ(tokens[6].id, CLOSE_BRACKET);
  EXPECT_EQ(tokens[7].id, SPACE);
  EXPECT_EQ(tokens[8].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[9].id, IDENTIFIER);
  EXPECT_EQ(tokens[10].id, SPACE);
  EXPECT_EQ(tokens[11].id, IDENTIFIER);
  EXPECT_EQ(tokens[12].id, SPACE);
  EXPECT_EQ(tokens[13].id, INTEGER);
  EXPECT_EQ(tokens[14].id, CLOSE_BRACKET);
  EXPECT_EQ(tokens[15].id, CLOSE_BRACKET);
  EXPECT_EQ(tokens[16].id, SPACE);
  EXPECT_EQ(tokens[17].id, INTEGER);
  EXPECT_EQ(tokens[18].id, CLOSE_BRACKET);
  EXPECT_EQ(tokens.size(), 19);
}

TEST(LispLambdaTests, LambdaParserTest2) {
  LispParser parser("((lambda (x) (+ x 10)) 22)");

  LispTokens tokens = parser.parse();

  LispValueParser value_parser(tokens);

  auto value = value_parser.next();
  EXPECT_TRUE(value);

  EXPECT_TRUE(value->is_list());

  LispValuePtrVector list = value->list();

  EXPECT_TRUE(list[0]->is_list());

  LispValuePtrVector lambda_list = list[0]->list();

  EXPECT_TRUE(lambda_list[0]->is_identifier());
  EXPECT_TRUE(lambda_list[1]->is_list());

  LispValuePtrVector params_list = lambda_list[1]->list();
  EXPECT_TRUE(params_list[0]->is_identifier());

  EXPECT_TRUE(lambda_list[2]->is_list());
  LispValuePtrVector definition_list = lambda_list[2]->list();
  EXPECT_TRUE(definition_list[0]->is_identifier());// +
  EXPECT_EQ(definition_list[0]->string(), "+");    // +
  EXPECT_TRUE(definition_list[1]->is_identifier());// x
  EXPECT_EQ(definition_list[1]->string(), "x");    // x
  EXPECT_TRUE(definition_list[2]->is_integer());   // 10
  EXPECT_EQ(definition_list[2]->to_integer(), 10); // 10

  EXPECT_TRUE(list[1]->is_integer());
  EXPECT_EQ(list[1]->to_integer(), 22);// 22
}

TEST(LispLambdaTests, LambdaExecutionTest1) {
  std::map<std::string, LispValuePtr> tests = {
      {"(funcall (lambda (x) (+ x 10)) 22)", LispValueFactory::new_double(32)},
      {"(funcall (lambda (x) (+ (- 100 x) 10)) 20)", LispValueFactory::new_double(90)},
  };

  return execute_lisp_tests(tests, "lambda");
}