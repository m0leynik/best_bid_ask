project(best_bid_ask_benchmark)

include(../cmake/google_benchmark.cmake)

include_directories(../lib/include/)

if (NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
endif()

if (NOT TARGET best_bid_ask_lib)
    add_subdirectory(../lib build)
endif()

add_executable(best_bid_ask_benchmark main.cpp)
target_link_libraries(best_bid_ask_benchmark benchmark::benchmark best_bid_ask_lib)
