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

#include "table_test_tools.h"
#include "model/table/lisp_execution_context_cell_reference.h"
#include "model/table/lisp_execution_context_cell_range.h"

void prepare_execution_context(LispExecutionContext *execution_context,
                               TableWorkbookDocumentPtr document) {
  execution_context->add_function(
      "cell", std::make_shared<LispExecutionContextCellReference>(document));
  execution_context->add_function(
      "cell_range", std::make_shared<LispExecutionContextCellRange>(document));
}


