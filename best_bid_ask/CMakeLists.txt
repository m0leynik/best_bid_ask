cmake_minimum_required(VERSION 3.20)
project(best_bid_ask)

set(CMAKE_CXX_STANDARD 20)

add_executable(best_bid_ask src/main.cpp)
include_directories(../lib/include/)
add_subdirectory(../lib build)
target_link_libraries(best_bid_ask best_bid_ask_lib)