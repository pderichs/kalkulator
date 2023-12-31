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

#ifndef MOCK_FUNCTION_CONTEXT_H
#define MOCK_FUNCTION_CONTEXT_H

#include "model/lisp/lisp_execution_context_error.h"
#include "model/lisp/lisp_function_execution_context.h"
#include "model/lisp/lisp_value.h"
#include "model/lisp/lisp_value_factory.h"
#include "gmock/gmock.h"

class MockFunctionContext : public LispFunctionExecutionContext {
public:
  MOCK_METHOD(LispValuePtr, value, (const LispValuePtrVector &func,
      const LispExecutionContext &execution_context,
      const std::any &context_param, UpdateIdType update_id), (override));
};

#endif // MOCK_FUNCTION_CONTEXT_H
