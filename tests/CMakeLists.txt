project(Google_tests)
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Kalkulator_tests target
add_executable(Kalkulator_tests TestTheTest.cpp)

target_link_libraries(Kalkulator_tests gtest gtest_main)