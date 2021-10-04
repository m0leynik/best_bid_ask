#include <benchmark/benchmark.h>

#include "bids_asks.h"
#include "helpers.h"

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
        m_fileContents = helpers::ReadEntireFile("./huobi_global_depth.log");
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
} // namespace

BENCHMARK_DEFINE_F(BestBidsAndAsksFixture, UnorderedMap_EvaluateBestBidsAndAsks)(benchmark::State& state)
{
    MapBasedEvaluateBestBidsAndAsksBenchmark<std::unordered_map<double, double>>(state, m_fileContents, m_outputStrategyStub);
}
BENCHMARK_REGISTER_F(BestBidsAndAsksFixture, UnorderedMap_EvaluateBestBidsAndAsks)->Iterations(100);


BENCHMARK_DEFINE_F(BestBidsAndAsksFixture, OrderedMap_EvaluateBestBidsAndAsks)(benchmark::State& state)
{
    MapBasedEvaluateBestBidsAndAsksBenchmark<std::map<double, double>>(state, m_fileContents, m_outputStrategyStub);
}
BENCHMARK_REGISTER_F(BestBidsAndAsksFixture, OrderedMap_EvaluateBestBidsAndAsks)->Iterations(100);



BENCHMARK_MAIN();