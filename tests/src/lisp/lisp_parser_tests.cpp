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
#include "model/lisp/lisp_execution_context_error.h"
#include "model/lisp/lisp_parser.h"
#include "model/lisp/lisp_value_parser.h"

TEST(LispParserTests, ParserTest1) {
  LispParser parser("(+ 22 4)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  EXPECT_EQ(tokens.size(), 7);
  EXPECT_EQ(tokens[0].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[1].id, IDENTIFIER);
  EXPECT_EQ(tokens[2].id, SPACE);
  EXPECT_EQ(tokens[3].id, INTEGER);
  EXPECT_EQ(tokens[4].id, SPACE);
  EXPECT_EQ(tokens[5].id, INTEGER);
  EXPECT_EQ(tokens[6].id, CLOSE_BRACKET);
}

TEST(LispParserTests, ParserTest2) {
  LispParser parser(R"((some-function "Hello \"World\"" 22 4))");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  EXPECT_EQ(tokens.size(), 9);
  EXPECT_EQ(tokens[0].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[1].id, IDENTIFIER);
  auto s = std::any_cast<std::string>(tokens[1].content);
  EXPECT_EQ(s, "some-function");
  EXPECT_EQ(tokens[2].id, SPACE);
  EXPECT_EQ(tokens[3].id, STRING);
  s = std::any_cast<std::string>(tokens[3].content);
  EXPECT_EQ(s, "Hello \\\"World\\\"");
  EXPECT_EQ(tokens[4].id, SPACE);
  EXPECT_EQ(tokens[5].id, INTEGER);
  auto d = std::any_cast<LispValue::IntegerType>(tokens[5].content);
  EXPECT_EQ(d, 22.0);
  EXPECT_EQ(tokens[6].id, SPACE);
  EXPECT_EQ(tokens[7].id, INTEGER);
  d = std::any_cast<LispValue::IntegerType>(tokens[7].content);
  EXPECT_EQ(d, 4.0);
  EXPECT_EQ(tokens[8].id, CLOSE_BRACKET);
}

TEST(LispParserTests, ParserTest3) {
  LispParser parser("(hello \"8282\" -484.32)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  EXPECT_EQ(tokens.size(), 7);
  EXPECT_EQ(tokens[0].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[1].id, IDENTIFIER);
  auto s = std::any_cast<std::string>(tokens[1].content);
  EXPECT_EQ(s, "hello");
  EXPECT_EQ(tokens[2].id, SPACE);
  EXPECT_EQ(tokens[3].id, STRING);
  s = std::any_cast<std::string>(tokens[3].content);
  EXPECT_EQ(s, "8282");
  EXPECT_EQ(tokens[4].id, SPACE);
  EXPECT_EQ(tokens[5].id, DOUBLE);
  auto d = std::any_cast<LispValue::DoubleType>(tokens[5].content);
  EXPECT_EQ(d, -484.32);
  EXPECT_EQ(tokens[6].id, CLOSE_BRACKET);
}

TEST(LispParserTests, ParserTest4) {
  LispParser parser("(funktion1 \"(* 5.344 22)\" \")\")");

  LispTokens tokens;

  EXPECT_NO_THROW(tokens = parser.parse());

  EXPECT_EQ(tokens.size(), 7);
  EXPECT_EQ(tokens[0].id, OPEN_BRACKET);
  EXPECT_EQ(tokens[1].id, IDENTIFIER);
  auto s = std::any_cast<std::string>(tokens[1].content);
  EXPECT_EQ(s, "funktion1");
  EXPECT_EQ(tokens[2].id, SPACE);
  EXPECT_EQ(tokens[3].id, STRING);
  s = std::any_cast<std::string>(tokens[3].content);
  EXPECT_EQ(s, "(* 5.344 22)");
  EXPECT_EQ(tokens[4].id, SPACE);
  EXPECT_EQ(tokens[5].id, STRING);
  s = std::any_cast<std::string>(tokens[5].content);
  EXPECT_EQ(s, ")");
  EXPECT_EQ(tokens[6].id, CLOSE_BRACKET);
}

TEST(LispParserTests, ExpressionTest1) {
  LispParser parser("(hello \"8282\" -484.32)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto func = value_parser.next();
  EXPECT_TRUE(func);

  LispValue val = *func;

  EXPECT_TRUE(val.is_function());
  LispValuePtrVector expr = val.list();

  LispValue::DoubleType d;
  std::string s;

  EXPECT_EQ(expr[0]->string(), "hello");
  EXPECT_EQ(expr.size(), 3);

  {
    const auto &value = expr[1];
    EXPECT_TRUE(value);
    EXPECT_TRUE(value->is_string());
    s = value->string();
    EXPECT_EQ(s, "8282");
  }

  {
    const auto &value = expr[2];
    EXPECT_TRUE(value);
    EXPECT_TRUE(value->is_number());
    d = value->to_double();
    EXPECT_EQ(d, -484.32);
  }
}

TEST(LispParserTests, ExpressionTest2) {
  LispParser parser("(hello (- 100 2 30) -484.32)");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto func = value_parser.next();
  EXPECT_TRUE(func);

  LispValue val = *func;
  EXPECT_TRUE(val.is_list());

  LispValuePtrVector expr = val.list();

  EXPECT_EQ(expr[0]->string(), "hello");
  // std::cerr << expr.param_count() << std::endl;
  EXPECT_EQ(expr.size(), 3);

  LispValuePtr param = expr[1];
  EXPECT_TRUE(param);

  EXPECT_TRUE(param->is_function());
  EXPECT_EQ(param->list()[0]->string(), "-");

  param = expr[2];
  EXPECT_TRUE(param);

  EXPECT_TRUE(param->is_number());
  EXPECT_EQ(param->to_double(), -484.32);
}

TEST(LispParserTests, WrongFormTests1) {
  StringVector cases = {
      "(",
      ")",
      "(343 1)",
      "(\"hello\" 1)",
      "123",
      "\"Hello\"",
  };

  for (const auto &c : cases) {
    std::cout << "Testing wrong form: " << c << std::endl;
    LispParser parser(c);

    LispTokens tokens;
    EXPECT_ANY_THROW(tokens = parser.parse());
  }
}

TEST(LispParserTests, ExpressionWithIdentifierTest1) {
  LispParser parser("(hello 33 some-variable \"Hello world\")");

  LispTokens tokens;
  EXPECT_NO_THROW(tokens = parser.parse());

  LispValueParser value_parser(tokens);

  auto func = value_parser.next();
  EXPECT_TRUE(func);

  LispValue val = *func;
  EXPECT_TRUE(val.is_list());

  LispValuePtrVector expr = val.list();

  EXPECT_EQ(expr[0]->string(), "hello");
  EXPECT_EQ(expr.size(), 4);

  LispValuePtr param = expr[1];
  EXPECT_TRUE(param);

  EXPECT_TRUE(param->is_number());
  EXPECT_EQ(param->to_integer(), 33);

  param = expr[2];
  EXPECT_TRUE(param);

  EXPECT_TRUE(param->is_identifier());
  EXPECT_EQ(param->string(), "some-variable");

  param = expr[3];
  EXPECT_TRUE(param);

  EXPECT_TRUE(param->is_string());
  EXPECT_EQ(param->string(), "Hello world");
}