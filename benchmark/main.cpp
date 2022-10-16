#include <benchmark/benchmark.h>

#include "bids_asks.h"
#include "helpers.h"

#define CONCATENATE(a, b) CONCATENATE_IMPL(a, b)
#define CONCATENATE_IMPL(a, b) a ## b


namespace {
class OutputStrategyStub final :
        public bids_asks::IOutputStrategy
{
public:
    void OnBestParametersEvaluated(const bids_asks::BestParameters &/*bestParams*/) override
    {}
};

class BestBidsAndAsksFixture :
        public ::benchmark::Fixture
{
public:
    void SetUp(const ::benchmark::State& /*st*/) override
    {
        m_fileContents = helpers::ReadEntireFile("./test/data/depth_of_market.log");
    }

protected:
    std::string m_fileContents {};
    OutputStrategyStub m_outputStrategyStub {};
};

template <
    bids_asks::map_of_orders_t map_t, 
    bids_asks::JsonProcessor PayloadProcessor
>
void MapBasedEvaluateBestBidsAndAsksBenchmark(
        benchmark::State& state,
        std::string_view fileContents,
        bids_asks::IOutputStrategy &outputStrategy)
{
    while (state.KeepRunning()) {
        using order_book_traits_t = bids_asks::order_book_traits<map_t>;
        bids_asks::EvaluateBestGeneric<order_book_traits_t, PayloadProcessor>(fileContents, outputStrategy);
    }
}

constexpr size_t BenchmarkIterations = 100u;

using small_flat_map = boost::container::small_flat_map<double, double, bids_asks::MaxOrders>;
} // namespace

#define MAP_BASED_BENCHMARK_SPECIFIC(map_t, payload_processor, benchmark_name) \
    BENCHMARK_DEFINE_F(BestBidsAndAsksFixture, benchmark_name)(benchmark::State& state)\
    {\
        MapBasedEvaluateBestBidsAndAsksBenchmark<map_t, payload_processor>(state, m_fileContents, m_outputStrategyStub);\
    }\
    BENCHMARK_REGISTER_F(BestBidsAndAsksFixture, benchmark_name)->Iterations(BenchmarkIterations)


#define MAP_BASED_BENCHMARK(map_t, payload_processor, benchmark_name)\
    using CONCATENATE(specific_map_t, __LINE__) = map_t<double, double>;\
    MAP_BASED_BENCHMARK_SPECIFIC(CONCATENATE(specific_map_t, __LINE__), payload_processor, benchmark_name)




// RapidJson
MAP_BASED_BENCHMARK(std::unordered_map, bids_asks::JsonProcessor::RapidJson, RapidJson_UnorderedMap_EvaluateBest);
MAP_BASED_BENCHMARK(std::map, bids_asks::JsonProcessor::RapidJson, RapidJson_OrderedMap_EvaluateBest);
MAP_BASED_BENCHMARK(boost::container::flat_map, bids_asks::JsonProcessor::RapidJson, RapidJson_BoostFlatMap_EvaluateBest);
MAP_BASED_BENCHMARK_SPECIFIC(small_flat_map, bids_asks::JsonProcessor::RapidJson, RapidJson_BoostSmallFlatMap_EvaluateBest);

// Nlohmann-Json
MAP_BASED_BENCHMARK(std::unordered_map, bids_asks::JsonProcessor::NlohmannJson, NlohmannJson_UnorderedMap_EvaluateBest);
MAP_BASED_BENCHMARK(std::map, bids_asks::JsonProcessor::NlohmannJson, NlohmannJson_OrderedMap_EvaluateBest);
MAP_BASED_BENCHMARK(boost::container::flat_map, bids_asks::JsonProcessor::NlohmannJson, NlohmannJson_BoostFlatMap_EvaluateBest);
MAP_BASED_BENCHMARK_SPECIFIC(small_flat_map, bids_asks::JsonProcessor::NlohmannJson, NlohmannJson_BoostSmallFlatMap_EvaluateBest);

BENCHMARK_MAIN();