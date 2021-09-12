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

void Benchmark_BidsAsks_EvaluateBest(benchmark::State& state)
{
    const auto fileContents = helpers::ReadEntireFile("./huobi_global_depth.log");
    OutputStrategyStub outputStrategyStub {};

    while (state.KeepRunning()) {
        bids_asks::EvaluateBest(fileContents, outputStrategyStub);
    }
}
} // namespace

BENCHMARK(Benchmark_BidsAsks_EvaluateBest)->Iterations(10);

BENCHMARK_MAIN();