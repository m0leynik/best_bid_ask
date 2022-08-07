#include <benchmark/benchmark.h>

#include "bids_asks.h"
#include "helpers.h"

#define CONCATINATE(a, b) CONCATINATE_IMPL(a, b)
#define CONCATINATE_IMPL(a, b) a ## b


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

template <typename map_t>
void MapBasedEvaluateBestBidsAndAsksBenchmark(
        benchmark::State& state,
        std::string_view fileContents,
        bids_asks::IOutputStrategy &outputStrategy)
{
    while (state.KeepRunning()) {
        bids_asks::EvaluateBest<map_t>(fileContents, outputStrategy);
    }
}

constexpr size_t BenchmarkIterations = 100u;
} // namespace

#define MAP_BASED_BENCHMARK_SPECIFIC(map_t, benchmark_name) \
    BENCHMARK_DEFINE_F(BestBidsAndAsksFixture, benchmark_name)(benchmark::State& state)\
    {\
        MapBasedEvaluateBestBidsAndAsksBenchmark<map_t>(state, m_fileContents, m_outputStrategyStub);\
    }\
    BENCHMARK_REGISTER_F(BestBidsAndAsksFixture, benchmark_name)->Iterations(BenchmarkIterations)


#define MAP_BASED_BENCHMARK(map_t, benchmark_name)\
    typedef map_t<double, double> CONCATINATE(specific_map_t, __LINE__);\
    MAP_BASED_BENCHMARK_SPECIFIC(CONCATINATE(specific_map_t, __LINE__), benchmark_name)

// unordered_map
MAP_BASED_BENCHMARK(std::unordered_map, UnorderedMap_EvaluateBestBidsAndAsks);
// map
MAP_BASED_BENCHMARK(std::map, OrderedMap_EvaluateBestBidsAndAsks);
// flat_map
MAP_BASED_BENCHMARK(boost::container::flat_map, BoostFlatMap_EvaluateBestBidsAndAsks);
// small_flat_map
typedef boost::container::small_flat_map<double, double, bids_asks::IOrderBook::MaxOrders> small_flat_map_order_book_t;
MAP_BASED_BENCHMARK_SPECIFIC(small_flat_map_order_book_t, BoostSmallFlatMap_EvaluateBestBidsAndAsks);


BENCHMARK_MAIN();