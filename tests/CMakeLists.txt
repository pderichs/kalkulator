cmake_minimum_required(VERSION 3.25)
project(kalkulator_tests LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES ../src/*.cpp ../src/*.h src/*.cpp src/*.h)

list(FILTER SOURCES EXCLUDE REGEX ".*src/main.cpp$")
message("Used sources: ${SOURCES}")
include_directories(../src ../src/model ../src/tools ../src/tests src)

# adding the Kalkulator_tests target
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} gtest gtest_main gmock gmock_main ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES})
