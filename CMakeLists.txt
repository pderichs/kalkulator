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
    src/view/table_control.h
    src/view/table_control.cpp
    src/view/cells_view_control.h
    src/view/cells_view_control.cpp
    src/view/table_row_headers_control.h
    src/view/table_row_headers_control.cpp
    src/view/table_column_headers_control.h
    src/view/table_column_headers_control.cpp
    src/view/table_sheet_view.h
    src/view/table_sheet_view.cpp
    src/view/table_formula_text_control.h
    src/view/table_formula_text_control.cpp
    src/view/table_cell_format_dlg.h
    src/view/table_cell_format_dlg.cpp
    src/model/table/table_sheet.h
    src/model/table/table_sheet.cpp
    src/model/table/table_cell.h
    src/model/table/table_cell.cpp
    src/model/table/table_workbook_document.h
    src/model/table/table_workbook_document.cpp
    src/model/table/table_workbook_file.h
    src/model/table/table_workbook_file.cpp
    src/model/lisp/lisp_parser.h
    src/model/lisp/lisp_parser.cpp
    src/model/lisp/lisp_tokens.h
    src/model/lisp/lisp_tokens.cpp
    src/model/lisp/lisp_execution_context.h
    src/model/lisp/lisp_execution_context.cpp
    src/model/lisp/lisp_function_execution_context.h
    src/model/lisp/lisp_function_execution_context.cpp
    src/model/lisp/lisp_value_parser.h
    src/model/lisp/lisp_value_parser.cpp
    src/model/lisp/lisp_syntax_checker.h
    src/model/lisp/lisp_syntax_checker.cpp
    src/model/lisp/value_converter.h
    src/model/lisp/value_converter.cpp
    src/tools/tools.h
    src/tools/tools.cpp
    src/tests/tests.h
    src/tests/tests.cpp
    src/tests/lisp/lisp_tests.h
    src/tests/lisp/lisp_tests.cpp
    src/tests/table/table_lisp_function_tests.h
    src/tests/table/table_lisp_function_tests.cpp
    src/tests/lisp/lisp_value_tests.h
    src/tests/lisp/lisp_value_tests.cpp
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
