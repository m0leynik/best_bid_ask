cmake_minimum_required(VERSION 3.20)
project(console_app)

set(CMAKE_CXX_STANDARD 20)

add_executable(console_app src/main.cpp)
include_directories(../lib/include/)
add_subdirectory(../lib build)
target_link_libraries(console_app best_bid_ask_lib)