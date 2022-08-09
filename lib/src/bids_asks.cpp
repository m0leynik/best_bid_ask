#include <functional>
#include <iomanip>
#include <map>
#include <ranges>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include <file_output_strategy.h>
#include <helpers.h>

#include "bids_asks.h"
#include "payload_tags.h"

namespace bids_asks {
namespace {
size_t FindSubstring(std::string_view line, std::string_view substr, size_t searchFrom = 0u)
{
    const auto position = line.find(substr, searchFrom);
    if (position == decltype(line)::npos) {
        throw std::invalid_argument(std::string("Character '") + std::string(substr) + "' not found");
    }
    return position;
}

std::string_view GetJsonPayloadFromLine(std::string_view line)
{
    // since format is simple and contains primitive JSON
    // with a singe set of {} simple search is used
    static const std::string JsonPayloadPrefix = "[exchange_tools] [info] Get Object: ";
    static const std::string JsonPayloadEnd = "}";

    const auto payloadStart = FindSubstring(line, JsonPayloadPrefix) + JsonPayloadPrefix.size();
    const auto payloadEnd  = FindSubstring(line, JsonPayloadEnd, payloadStart) + 1u;

    return line.substr(payloadStart, payloadEnd - payloadStart);
}

BestParameters EvaluateBestParametersFromJsonPayload(
    std::string_view jsonPayload,
    IPayloadProcessor &payloadProcessor,
    const IOrderBook &orderBook)
{
    payloadProcessor.Process(jsonPayload);

    return orderBook.GetBestParameters();
}

BestParameters EvaluateBestParametersByLine(
    std::string_view line,
    IPayloadProcessor &payloadProcessor,
    const IOrderBook &orderBook)
{
    const auto jsonPayload = GetJsonPayloadFromLine(line);

    return EvaluateBestParametersFromJsonPayload(jsonPayload, payloadProcessor, orderBook);
}

std::string_view SplitViewToStringView(const auto & splitView)
{
    return std::string_view(&*splitView.begin(), std::ranges::distance(splitView));
}

} // namespace

void EvaluateBest(
        std::string_view fileContents,
        IPayloadProcessor &payloadProcessor,
        const IOrderBook &orderBook,
        IOutputStrategy &outputStrategy)
{
    // enumerate lines
    for (const auto line : std::views::split(fileContents, '\n')) {
        outputStrategy.OnBestParametersEvaluated(
                EvaluateBestParametersByLine(
                        SplitViewToStringView(line), payloadProcessor, orderBook));
    }
}

void EvaluateBest(std::string_view inputFile, std::string_view outputFile)
{
    const auto fileContents = helpers::ReadEntireFile(inputFile);
    const auto outputStrategy = bids_asks::CreateFileOutputStrategy(outputFile);

    EvaluateBest(fileContents, *outputStrategy);
}

void EvaluateBest(std::string_view fileContents, IOutputStrategy &outputStrategy)
{
    using default_container_t = boost::container::small_flat_map<double, double, bids_asks::IOrderBook::MaxOrders>;

    EvaluateBestGeneric<default_container_t, JsonProcessor::RapidJson>(fileContents, outputStrategy);
}

} // bids_asks