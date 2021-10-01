cmake_minimum_required(VERSION 3.20)
project(best_bid_ask)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(./console_app)
add_subdirectory(./benchmark)
add_subdirectory(./test)
