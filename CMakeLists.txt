cmake_minimum_required(VERSION 3.16)
project(SmartCampusPlanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(campus_core src/Scheduler.cpp src/Storage.cpp)
target_include_directories(campus_core PUBLIC include)

if(MSVC)
    target_compile_options(campus_core PRIVATE /W4)
else()
    target_compile_options(campus_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(smart-campus src/main.cpp)
target_link_libraries(smart-campus PRIVATE campus_core)

enable_testing()
add_executable(scheduler_tests tests/scheduler_tests.cpp)
target_link_libraries(scheduler_tests PRIVATE campus_core)
add_test(NAME scheduler_tests COMMAND scheduler_tests)
