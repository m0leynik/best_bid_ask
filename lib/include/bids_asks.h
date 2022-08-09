#ifndef BEST_BID_ASK_LIB_BIDS_ASKS_H
#define BEST_BID_ASK_LIB_BIDS_ASKS_H

#include <string_view>

#include <nlohmann_json_payload_processor.h>
#include <rapidjson_payload_processor.h>
#include <order_book.h>
#include <output_strategy_iface.h>

namespace bids_asks {

using default_container_t = boost::container::small_flat_map<double, double, bids_asks::IOrderBook::MaxOrders>;;

enum class JsonProcessor
{
    NlohmanJson,
    RapidJson
};

void EvaluateBest(
        std::string_view fileContents,
        IPayloadProcessor &payloadProcessor,
        const IOrderBook &orderBook,
        IOutputStrategy &outputStrategy);

void EvaluateBest(std::string_view inputFile, std::string_view outputFile);
void EvaluateBest(std::string_view fileContents, IOutputStrategy &outputStrategy);

template <typename container_t, JsonProcessor jsonProcessor>
void EvaluateBestGeneric(
    std::string_view fileContents, 
    IOutputStrategy &outputStrategy)
{
    const auto orderBook = std::make_shared<OrderBookActualizer<container_t>>();
    std::unique_ptr<IPayloadProcessor> payloadProcessor {};

    if constexpr (jsonProcessor == JsonProcessor::RapidJson) {
        payloadProcessor = CreateRapidJsonPayloadProcessor(orderBook);
    } else if constexpr (jsonProcessor == JsonProcessor::NlohmanJson) {
        payloadProcessor = CreateNlohmannJsonPayloadProcessor(orderBook);
    } else {
        static_assert(
            jsonProcessor == JsonProcessor::NlohmanJson || jsonProcessor == JsonProcessor::RapidJson, 
            "Invalid json payload processor type provided");
        return;
    }

    EvaluateBest(fileContents, *payloadProcessor, *orderBook, outputStrategy);
}

} // bids_asks
#endif //BEST_BID_ASK_LIB_BIDS_ASKS_H
