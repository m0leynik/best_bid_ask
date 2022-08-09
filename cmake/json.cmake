include(FetchContent)
FetchContent_Declare(nlohmann-json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.1)

FetchContent_MakeAvailable(nlohmann-json)


FetchContent_Declare(rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG v1.1.0)

FetchContent_MakeAvailable(rapidjson)