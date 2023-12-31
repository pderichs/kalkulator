cmake_minimum_required(VERSION 3.25)
project(kalkulator_tests LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

find_package(wxWidgets COMPONENTS core base REQUIRED)
include(${wxWidgets_USE_FILE})

find_package(SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES ../src/*.cpp ../src/*.h src/*.cpp src/*.h)
list(FILTER SOURCES EXCLUDE REGEX ".*src/main.cpp$")
message("Used sources: ${SOURCES}")
include_directories(../src ../src/model ../src/tools ../src/tests src)

# adding the Kalkulator_tests target
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock gmock_main ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES})