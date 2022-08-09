project(best_bid_ask_lib)

include(../cmake/json.cmake)

include_directories(
        ./include/
        ${rapidjson_SOURCE_DIR}/include)
add_library(best_bid_ask_lib src/bids_asks.cpp include/file_output_strategy.h src/file_output_strategy.cpp
        include/payload_tags.h include/payload_processor_iface.h
        include/nlohmann_json_payload_processor.h src/nlohmann_json_payload_processor.cpp
        include/rapidjson_payload_processor.h src/rapidjson_payload_processor.cpp
        include/output_strategy_iface.h include/best_parameters.h
        include/order_book.h include/order_book_iface.h)
        
target_link_libraries(best_bid_ask_lib PRIVATE nlohmann_json::nlohmann_json)