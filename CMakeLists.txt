cmake_minimum_required(VERSION 3.7.2)
cmake_policy(VERSION 3.7.2)
project(ovenbrick)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(contrib)

set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.58.0 REQUIRED COMPONENTS program_options log)

add_definitions(-DBOOST_LOG_DYN_LINK)
add_subdirectory(game)

add_executable(ovenbrick main.cxx)
target_link_libraries(ovenbrick PRIVATE game)
target_link_libraries(ovenbrick PRIVATE Boost::log Boost::program_options)

enable_testing()

add_subdirectory(test)
