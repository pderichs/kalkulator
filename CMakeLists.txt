cmake_minimum_required(VERSION 3.25)
project(kalkulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# from: https://cmake.org/cmake/help/latest/command/add_compile_options.html
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

set(SOURCES
	src/main.cpp
	src/model/event_sink.h
	src/model/lisp/lisp_execution_context.cpp
	src/model/lisp/lisp_execution_context.h
	src/model/lisp/lisp_execution_context_addition.h
	src/model/lisp/lisp_execution_context_and.h
	src/model/lisp/lisp_execution_context_average.h
	src/model/lisp/lisp_execution_context_cons.h
	src/model/lisp/lisp_execution_context_division.h
	src/model/lisp/lisp_execution_context_eq.h
	src/model/lisp/lisp_execution_context_error.h
	src/model/lisp/lisp_execution_context_first.h
	src/model/lisp/lisp_execution_context_floor.h
	src/model/lisp/lisp_execution_context_funcall.h
	src/model/lisp/lisp_execution_context_if.h
	src/model/lisp/lisp_execution_context_join.h
	src/model/lisp/lisp_execution_context_lambda.h
	src/model/lisp/lisp_execution_context_list.h
	src/model/lisp/lisp_execution_context_multiplication.h
	src/model/lisp/lisp_execution_context_not.h
	src/model/lisp/lisp_execution_context_or.h
	src/model/lisp/lisp_execution_context_progn.h
	src/model/lisp/lisp_execution_context_rest.h
	src/model/lisp/lisp_execution_context_subtraction.h
	src/model/lisp/lisp_execution_context_xor.h
	src/model/lisp/lisp_function_definition.h
	src/model/lisp/lisp_function_execution_context.cpp
	src/model/lisp/lisp_function_execution_context.h
	src/model/lisp/lisp_lambda_executor.h
	src/model/lisp/lisp_parser.cpp
	src/model/lisp/lisp_parser.h
	src/model/lisp/lisp_parser_error.h
	src/model/lisp/lisp_runtime_error.h
	src/model/lisp/lisp_syntax_checker.cpp
	src/model/lisp/lisp_syntax_checker.h
	src/model/lisp/lisp_tokens.cpp
	src/model/lisp/lisp_tokens.h
	src/model/lisp/lisp_value.h
	src/model/lisp/lisp_value_factory.h
	src/model/lisp/lisp_value_parser.cpp
	src/model/lisp/lisp_value_parser.h
	src/model/lisp/lisp_value_ptr.h
	src/model/lisp/value_conversion_error.h
	src/model/lisp/value_converter.cpp
	src/model/lisp/value_converter.h
	src/model/table/lisp_execution_context_cell_range.h
	src/model/table/lisp_execution_context_cell_reference.h
	src/model/table/table_cell.cpp
	src/model/table/table_cell.h
	src/model/table/table_cell_color.h
	src/model/table/table_cell_format.h
	src/model/table/table_cell_orientation.h
	src/model/table/table_change_history.h
	src/model/table/table_column_definition.h
	src/model/table/table_row_definition.h
	src/model/table/table_selections.h
	src/model/table/table_sheet.cpp
	src/model/table/table_sheet.h
	src/model/table/table_workbook_document.cpp
	src/model/table/table_workbook_document.h
	src/model/table/table_workbook_file.cpp
	src/model/table/table_workbook_file.h
	src/model/table/table_workbook_file_error.h
	src/tests/lisp/lisp_tests.cpp
	src/tests/lisp/lisp_tests.h
	src/tests/lisp/lisp_value_tests.cpp
	src/tests/lisp/lisp_value_tests.h
	src/tests/table/table_lisp_function_tests.cpp
	src/tests/table/table_lisp_function_tests.h
	src/tests/tests.cpp
	src/tests/tests.h
	src/tools/location.h
	src/tools/rectangle.h
	src/tools/tools.cpp
	src/tools/tools.h
	src/view/cells_view_control.cpp
	src/view/cells_view_control.h
	src/view/kalkulator_system_colors.h
	src/view/table_cell_format_dlg.cpp
	src/view/table_cell_format_dlg.h
	src/view/table_column_headers_control.cpp
	src/view/table_column_headers_control.h
	src/view/table_control.cpp
	src/view/table_control.h
	src/view/table_formula_text_control.cpp
	src/view/table_formula_text_control.h
	src/view/table_row_headers_control.cpp
	src/view/table_row_headers_control.h
	src/view/table_sheet_view.cpp
	src/view/table_sheet_view.h
)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC src src/tests src/tools)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES})

# TODO
# enable_testing()
# add_subdirectory(src/tests)
