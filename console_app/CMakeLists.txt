project(console_app)

add_executable(console_app src/main.cpp)
include_directories(../lib/include/)

if (NOT TARGET best_bid_ask_lib)
    add_subdirectory(../lib build)
endif()

target_link_libraries(console_app best_bid_ask_lib)