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
    add_compile_options(-Wall -Wpedantic -Wextra -Weffc++ -Werror)
endif()

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES src/*.cpp src/*.h)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC src src/tests src/tools)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES})

# TODO
# enable_testing()
# add_subdirectory(src/tests)
