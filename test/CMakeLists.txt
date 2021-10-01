project(test)

include(../cmake/google_benchmark.cmake)

enable_testing()
add_executable(test src/main.cpp src/best_bid_ask_test.cpp include/output_strategy_mock.h include/payload_processor_mock.h)
include_directories(../lib/include/ ./include)

if (NOT TARGET best_bid_ask_lib)
    add_subdirectory(../lib build)
endif()

target_link_libraries(test best_bid_ask_lib gmock_main)
