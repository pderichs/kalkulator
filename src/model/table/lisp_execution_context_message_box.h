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

#ifndef LISP_EXECUTION_CONTEXT_MESSAGE_BOX_H
#define LISP_EXECUTION_CONTEXT_MESSAGE_BOX_H

#include "../event_sink.h"
#include "model/lisp/lisp_common_values.h"
#include "model/lisp/lisp_execution_context.h"
#include "model/lisp/lisp_function_execution_context.h"

class LispExecutionContextMessageBox : public LispFunctionExecutionContext {
public:
  LispExecutionContextMessageBox(EventSink *event_sink)
      : _event_sink(event_sink) {}
  ~LispExecutionContextMessageBox() override = default;

  // Delete copy constructor and assignment operator
  LispExecutionContextMessageBox(const LispExecutionContextMessageBox &other) = delete;
  LispExecutionContextMessageBox &operator=(const LispExecutionContextMessageBox &other) = delete;

  LispValuePtr value(const LispValuePtrVector &func,
                     const LispExecutionContext &execution_context,
                     const std::any &context_param,
                     UpdateIdType update_id) override {
    LispValuePtrVector params =
        extract_and_execute_params(func,
                                   execution_context,
                                   context_param,
                                   update_id);

    if (params.size() != 1) {
      return LispCommonValues::error_parameter_count();
    }

    if (!params[0]->is_string()) {
      return LispCommonValues::error_parameter();
    }

    std::string msg(params[0]->string());

    _event_sink->send_event(MESSAGE_BOX, msg);

    return params[0];
  }

private:
  EventSink *_event_sink;
};

#endif //LISP_EXECUTION_CONTEXT_MESSAGE_BOX_H
