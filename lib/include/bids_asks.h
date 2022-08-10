#ifndef BEST_BID_ASK_LIB_BIDS_ASKS_H
#define BEST_BID_ASK_LIB_BIDS_ASKS_H

#include <string_view>

#include <nlohmann_json_payload_processor.h>
#include <rapidjson_payload_processor.h>
#include <order_book.h>
#include <output_strategy_iface.h>

namespace bids_asks {

enum class JsonProcessor
{
    NlohmannJson,
    RapidJson
};

void EvaluateBest(std::string_view inputFile, std::string_view outputFile);
void EvaluateBest(std::string_view fileContents, IOutputStrategy &outputStrategy);
void EvaluateBest(
        std::string_view fileContents,
        IPayloadProcessor &payloadProcessor,
        const IOrderBook &orderBook,
        IOutputStrategy &outputStrategy);


template<JsonProcessor jsonProcessor>
std::unique_ptr<IPayloadProcessor> CreateJsonProcessor(std::shared_ptr<IPayloadProcessorCallback> payloadProcessorCallback)
{
    if constexpr (jsonProcessor == JsonProcessor::RapidJson) {
        return CreateRapidJsonPayloadProcessor(std::move(payloadProcessorCallback));
    } else if constexpr (jsonProcessor == JsonProcessor::NlohmannJson) {
        return CreateNlohmannJsonPayloadProcessor(std::move(payloadProcessorCallback));
    } else {
        static_assert(
            jsonProcessor == JsonProcessor::NlohmannJson || jsonProcessor == JsonProcessor::RapidJson, 
            "Invalid json payload processor type provided");
        return nullptr;
    }
}

template <typename container_t, JsonProcessor jsonProcessor>
void EvaluateBestGeneric(
    std::string_view fileContents, 
    IOutputStrategy &outputStrategy)
{
    const auto orderBook = std::make_shared<OrderBookActualizer<container_t>>();
    std::unique_ptr<IPayloadProcessor> payloadProcessor = CreateJsonProcessor<jsonProcessor>(orderBook);

    EvaluateBest(fileContents, *payloadProcessor, *orderBook, outputStrategy);
}

} // bids_asks
#endif //BEST_BID_ASK_LIB_BIDS_ASKS_H
