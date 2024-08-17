cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(tests)

