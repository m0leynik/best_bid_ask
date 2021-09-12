#include <functional>
#include <iomanip>
#include <map>
#include <ranges>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "bids_asks.h"
#include "file_output_strategy.h"
#include "helpers.h"

namespace {
size_t FindSubstring(std::string_view line, std::string_view substr, size_t searchFrom = 0)
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
    const auto payloadEnd  = FindSubstring(line, JsonPayloadEnd, payloadStart) + 1;

    return line.substr(payloadStart, payloadEnd - payloadStart);
}

const auto & GetBidsFromJson(const nlohmann::json &j)
{
    return j["bids"].get_ref<const nlohmann::json::array_t &>();
}

const auto & GetAsksFromJson(const nlohmann::json &j)
{
    return j["asks"].get_ref<const nlohmann::json::array_t &>();
}

std::pair<double, double> GetPriceAndAmount(const nlohmann::json &j)
{
    const auto & array = j.get_ref<const nlohmann::json::array_t &>();
    if (array.size() != 2) {
        throw std::invalid_argument(std::string("invalid size of json array: ") + to_string(j));
    }

    double price = array[0].get<nlohmann::json::number_float_t>();
    double amount = array[1].get<nlohmann::json::number_float_t>();

    return { price, amount };
}

struct OrderBook
{
    // unordered_map performs a bit better than map in concrete task case
    // <price, amount>
    using container_t = std::unordered_map<double, double>;

    container_t bids {};
    container_t asks {};
};

void UpdateOrder(const typename nlohmann::json::basic_json &jsonOrder, OrderBook::container_t &orders)
{
    const auto [price, amount] = GetPriceAndAmount(jsonOrder);

    if (amount == double(0.0)) {
        orders.erase(price);
        return;
    }
    // TODO: (?) use map::insert() parameter (hint) to optimize insertion
    orders[price] = amount;
}

void UpdateOrders(const nlohmann::json::array_t & jsonOrders, OrderBook::container_t &orders)
{
    for (const auto &jsonOrder : jsonOrders) {
        UpdateOrder(jsonOrder, orders);
    }
}

void FillBidsAndAsksFromUpdate(const nlohmann::json &j, OrderBook &orderBook)
{
    UpdateOrders(GetBidsFromJson(j), orderBook.bids);
    UpdateOrders(GetAsksFromJson(j), orderBook.asks);
}

void FillBidsAndAsksFromSnapshot(const nlohmann::json &j, OrderBook &orderBook)
{
    if (!orderBook.asks.empty() || !orderBook.bids.empty()) {
        throw std::invalid_argument("bids and asks expected to be empty since snapshot line must be first");
    }

    // bids are sorted from high to low price
    const auto& bids = GetBidsFromJson(j);
    for (const auto &bid : bids) {
        // TODO: (?) use hint as insert() parameter to optimize insertion
        orderBook.bids.insert(GetPriceAndAmount(bid));
    }

    // asks are sorted from low to high price
    const auto& asks = GetAsksFromJson(j);
    for (const auto &ask : asks) {
        orderBook.asks.insert(GetPriceAndAmount(ask));
    }
}

void UpdateBidsAndAsks(const nlohmann::json &j, OrderBook &orderBook)
{
    const auto &type = j["type"].get_ref<const nlohmann::json::string_t&>();

    if (type == "snapshot") {
        return FillBidsAndAsksFromSnapshot(j, orderBook);
    } else if (type == "update") {
        return FillBidsAndAsksFromUpdate(j, orderBook);
    } else {
        throw std::invalid_argument(
                std::string("invalid \"type\" value in json payload: ") + to_string(j));
    }
}

template <typename T>
auto GetSmallest(const std::map<T, T> &container)
{
    return container.cbegin();
}

template <typename T>
auto GetGreatest(const std::map<T, T> &container)
{
    return container.crbegin();
}

template <typename T>
auto GetSmallest(const std::unordered_map<T, T> &container)
{
    return std::min_element(container.cbegin(), container.cend(),
                            [](const auto& l, const auto& r) { return l.first < r.first; });
}

template <typename T>
auto GetGreatest(const std::unordered_map<T, T> &container)
{
    return std::max_element(container.cbegin(), container.cend(),
                            [](const auto& l, const auto& r) { return l.first < r.first; });
}

bids_asks::BestParameters EvaluateBestParametersFromJsonPayload(
    std::string_view jsonPayload,
    OrderBook &orderBook)
{
    const nlohmann::json j = nlohmann::json::parse(jsonPayload);
    UpdateBidsAndAsks(j, orderBook);

    bids_asks::BestParameters bestParams {};
    bestParams.eventTime = j["event_time"];

    const auto bestAsk = GetSmallest(orderBook.asks);
    bestParams.bestAskPrice = bestAsk->first;
    bestParams.bestAskAmount = bestAsk->second;

    const auto bestBid = GetGreatest(orderBook.bids);
    bestParams.bestBidPrice = bestBid->first;
    bestParams.bestBidAmount = bestBid->second;

    return bestParams;
}

bids_asks::BestParameters EvaluateBestParametersByLine(
    std::string_view line,
    OrderBook &orderBook)
{
    const auto jsonPayload = GetJsonPayloadFromLine(line);

    return EvaluateBestParametersFromJsonPayload(jsonPayload, orderBook);
}

/*void DumpPriceAndAmount(double price, double amount, std::ofstream &dump)
{
    dump << std::setprecision(2) << price << " " << std::setprecision(6) << amount << std::endl;
}

// suitable only for std::map containers
void DumpOrderBook(OrderBook &orderBook)
{
    std::ofstream dump("./order_book.txt", std::ios_base::out | std::ios_base::trunc);
    dump << std::fixed;

    dump << "asks:" << std::endl;
    for (auto i = orderBook.asks.crbegin(); i != orderBook.asks.crend(); ++i) {
        DumpPriceAndAmount(i->first, i->second, dump);
    }

    dump << "bids:" << std::endl;
    for (auto i = orderBook.bids.crbegin(); i != orderBook.bids.crend(); ++i) {
        DumpPriceAndAmount(i->first, i->second, dump);
    }
}*/

std::string_view SplitViewToStringView(const auto & splitView)
{
    return std::string_view(&*splitView.begin(), std::ranges::distance(splitView));
}

} // namespace

namespace bids_asks {
void EvaluateBest(std::string_view fileContents, bids_asks::IOutputStrategy &outputStrategy)
{
    OrderBook orderBook {};

    // enumerate lines
    for (const auto line : std::views::split(fileContents, '\n')) {
        outputStrategy.OnBestParametersEvaluated(
                EvaluateBestParametersByLine(
                        SplitViewToStringView(line), orderBook));
    }

    //DumpOrderBook(orderBook);
}

void EvaluateBest(std::string_view inputFile, std::string_view outputFile)
{
    const std::string fileContents = helpers::ReadEntireFile(inputFile);
    const auto outputStrategy = bids_asks::CreateFileOutputStrategy(outputFile);

    EvaluateBest(fileContents, *outputStrategy);
}
} // bids_asks