project(best_bid_ask_lib)

include_directories(./include/)
include(../cmake/json.cmake)
add_library(best_bid_ask_lib src/bids_asks.cpp src/file_output_strategy.cpp include/file_output_strategy.h)
target_link_libraries(best_bid_ask_lib PRIVATE nlohmann_json::nlohmann_json)