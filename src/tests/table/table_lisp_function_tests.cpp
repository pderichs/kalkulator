#include <any>
#include <tuple>

#include "../../model/table/lisp_execution_context_average.h"
#include "../../model/table/lisp_execution_context_cell_range.h"
#include "../../model/table/lisp_execution_context_cell_reference.h"
#include "../../model/table/table_workbook_document.h"
#include "table_lisp_function_tests.h"
#include "tools.h"
int run_cell_tests1();

// TODO cell_range

class TestEventSink : public EventSink {
public:
  TestEventSink() = default;
  virtual ~TestEventSink() = default;

  virtual void send_event(TableEvent event_id, std::any param) {
    std::ignore = event_id;
    std::ignore = param;
    // TODO?
  }
};

void prepare_execution_context(LispExecutionContext *execution_context,
                               TableWorkbookDocument *document);

int run_table_lisp_function_tests() {
  RUN_TEST(run_cell_tests1);

  return 0;
}

int run_cell_tests1() {
  // Test setup
  TestEventSink sink;
  LispExecutionContext execution_context;
  ValueConverter::update_execution_context(&execution_context);
  TableWorkbookDocument document(&sink);
  prepare_execution_context(&execution_context, &document);

  // Put value in cell 0 0
  document.select_cell(Location(0, 0));
  document.update_content_current_cell("42");

  // Put cell formula in cell 0 1
  document.select_cell(Location(0, 1));
  document.update_content_current_cell("=(cell 0 0)");

  const auto &opt_cell = document.get_cell(Location(0, 1));
  TEST_ASSERT(opt_cell);
  const auto &cell = *opt_cell;
  TEST_ASSERT(cell);
  LispValuePtr value = cell->lisp_value();
  TEST_ASSERT(value->is_function());
  TEST_ASSERT(cell->visible_content() == "42");

  return 0;
}

void prepare_execution_context(LispExecutionContext *execution_context,
                               TableWorkbookDocument *document) {
  execution_context->add_function(
      "cell", std::make_shared<LispExecutionContextCellReference>(document));
  execution_context->add_function(
      "cell_range", std::make_shared<LispExecutionContextCellRange>(document));
  execution_context->add_function(
      "avg", std::make_shared<LispExecutionContextAvg>(document));
}
