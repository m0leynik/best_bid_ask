#ifndef BEST_BID_ASK_LIB_BIDS_ASKS_H
#define BEST_BID_ASK_LIB_BIDS_ASKS_H

#include <string_view>

#include <nlohmann_json_payload_processor.h>
#include <order_book.h>
#include <output_strategy_iface.h>

namespace bids_asks {
void EvaluateBest(
        std::string_view fileContents,
        IPayloadProcessor &payloadProcessor,
        const IOrderBook &orderBook,
        IOutputStrategy &outputStrategy);

void EvaluateBest(std::string_view inputFile, std::string_view outputFile);

template <typename container_t = boost::container::flat_map<double, double>>
void EvaluateBest(std::string_view fileContents, IOutputStrategy &outputStrategy)
{
    const auto orderBook = std::make_shared<OrderBookActualizer<container_t>>();
    const auto payloadProcessor = CreateNlohmannJsonPayloadProcessor(orderBook);

    EvaluateBest(fileContents, *payloadProcessor, *orderBook, outputStrategy);
}
} // bids_asks
#endif //BEST_BID_ASK_LIB_BIDS_ASKS_H
