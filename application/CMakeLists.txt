project(application)

add_executable(application src/main.cpp)
include_directories(../lib/include/)

if (NOT TARGET best_bid_ask_lib)
    add_subdirectory(../lib build)
endif()

target_link_libraries(application best_bid_ask_lib)
