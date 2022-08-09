cmake_minimum_required(VERSION 3.16)
project(best_bid_ask)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# to silence googlebenchmark building error
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -fconcepts-diagnostics-depth=8")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(./application)
add_subdirectory(./benchmark)
add_subdirectory(./test)
