cmake_minimum_required(VERSION 3.25)
project(kalkulator LANGUAGES CXX)

file(GLOB_RECURSE SOURCES **.cpp **.h)

add_executable(
        ${PROJECT_NAME}
        ${SOURCES}
)

# from: https://cmake.org/cmake/help/latest/command/add_compile_options.html
if (MSVC)
    # warning level 4 and all warnings as errors
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
    # lots of warnings and all warnings as errors
    #add_compile_options(-Wall -Wpedantic -Wextra -Werror)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Weffc++ -Wextra -Werror)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC . ./tests ./tools)
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES})
