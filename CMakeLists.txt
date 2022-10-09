cmake_minimum_required(VERSION 3.16)
project(best_bid_ask)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# provide c++ concept diagnostics
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts-diagnostics-depth=8")
# silence googlebenchmark building errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
# silence rapidjson building errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-class-memaccess -Wno-implicit-fallthrough")
# debug info
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(RAPIDJSON_BUILD_TESTS  OFF CACHE BOOL "Build rapidjson perftests and unittests." FORCE)

add_subdirectory(./application)
add_subdirectory(./benchmark)
add_subdirectory(./test)
