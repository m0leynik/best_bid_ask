cmake_minimum_required(VERSION 3.20)
project(best_bid_ask_benchmark)

set(CMAKE_CXX_STANDARD 20)

include(../cmake/google_benchmark.cmake)

include_directories(../lib/include/)
add_subdirectory(../lib build)
add_executable(best_bid_ask_benchmark main.cpp)
target_link_libraries(best_bid_ask_benchmark benchmark::benchmark best_bid_ask_lib)