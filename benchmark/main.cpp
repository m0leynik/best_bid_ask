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
    void SetUp(const ::benchmark::State& st) override
    {
        m_fileContents = helpers::ReadEntireFile("./huobi_global_depth.log");
    }

protected:
    std::string m_fileContents {};
    OutputStrategyStub m_outputStrategyStub {};
};
} // namespace

BENCHMARK_DEFINE_F(BestBidsAndAsksFixture, BidsAndAsks)(benchmark::State& state)
{
    while (state.KeepRunning()) {
        bids_asks::EvaluateBest(m_fileContents, m_outputStrategyStub);
    }
}

BENCHMARK_REGISTER_F(BestBidsAndAsksFixture, BidsAndAsks)->Iterations(100);

BENCHMARK_MAIN();