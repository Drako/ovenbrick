cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(ovenbrick)

set(CMAKE_CXX_STANDARD 14)

enable_testing()

add_subdirectory(contrib)
add_subdirectory(game)
add_subdirectory(test)

add_executable(ovenbrick main.cxx)
target_link_libraries(ovenbrick game)
