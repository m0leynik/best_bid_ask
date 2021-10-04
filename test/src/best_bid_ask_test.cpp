#include <array>

#include "output_strategy_mock.h"
#include <gtest/gtest.h>

#include "bids_asks.h"

using namespace bids_asks;
using namespace testing;

namespace test_data{
constexpr auto *SnapshotLine =
        "[2020-08-23 19:19:29.137] [exchange_tools] [info] Get Object: "
        "{\"type\": \"snapshot\",\"event_time\": 1598210366838325,"
        "\"asks\": [[11666.15, 0.333936], [11666.39, 0.139009], [11668.18, 0.064259], "
        "[11668.29, 0.1939], [11668.3, 0.128553], [11668.7, 0.02], [11668.81, 0.01], "
        "[11669.16, 0.2], [11669.17, 0.1194], [11669.18, 2.0], [11669.24, 0.005226], "
        "[11669.25, 14.099585151809242], [11669.41, 0.009743], [11669.42, 0.14], "
        "[11669.52, 0.1], [11669.61, 0.024292], [11669.64, 0.153637], [11669.66, 0.43], "
        "[11669.74, 0.010207], [11669.75, 0.089344]],"
        "\"bids\": [[11666.14, 6.731865], [11665.91, 0.008087], [11665.84, 0.148], "
        "[11665.68, 0.1997], [11665.67, 0.008087], [11665.63, 0.139009], [11665.45, 0.3998], "
        "[11665.44, 0.008088], [11665.41, 0.14], [11665.21, 0.008088], [11665.15, 0.01], "
        "[11664.97, 0.008088], [11664.85, 0.0873], [11664.74, 0.008088], [11664.7, 0.024008], "
        "[11664.51, 0.008088], [11664.27, 0.008088], [11664.11, 3.956437], "
        "[11664.04, 0.008089], [11663.91, 0.042857]]}";
constexpr auto* UpdateLine =
        "[2020-08-23 19:19:29.137] [exchange_tools] [info] Get Object: "
        "{\"type\": \"update\",\"event_time\": 1598210366838326,"
        "\"asks\": [[11666.15, 0.0], [11666.39, 0.139009], [11668.18, 0.064259], "
        "[11668.29, 0.1939], [11668.3, 0.128553], [11668.7, 0.02], [11668.81, 0.01]],"
        "\"bids\": [[11666.14, 0.0], [11665.91, 0.008087], [11665.84, 0.148], "
        "[11665.68, 0.1997], [11665.67, 0.008087], [11665.63, 0.139009], [11665.45, 0.3998], "
        "[11665.44, 0.008088], [11665.41, 0.14], [11665.21, 0.008088], [11665.15, 0.01]]}";
}

TEST(EvaluateBest, CorrectEvaluationFromSnapshot)
{
    OutputStrategyMock outputStrategyMock{};
    const BestParameters expectedResult {11666.14, 6.731865, 11666.15, 0.333936, 1598210366838325};

    EXPECT_CALL(outputStrategyMock, OnBestParametersEvaluated(_)).WillOnce(
            [&](const BestParameters& bestParam){
                EXPECT_EQ(bestParam, expectedResult);
            }
    );
    EvaluateBest(test_data::SnapshotLine, outputStrategyMock);
}

TEST(EvaluateBest, CorrectEvaluationFromSnapshotAndUpdate)
{
    constexpr std::array<const char *, 2> Lines = { test_data::SnapshotLine, test_data::UpdateLine };
    constexpr std::array<BestParameters, 2> Results = {
        BestParameters{11666.14, 6.731865, 11666.15, 0.333936, 1598210366838325},
        BestParameters{ 11665.91, 0.008087, 11666.39, 0.139009, 1598210366838326}
    };
    OutputStrategyMock outputStrategyMock{};

    auto i = 0u;
    EXPECT_CALL(outputStrategyMock, OnBestParametersEvaluated(_)).Times(Lines.size()).WillRepeatedly(
            [&](const BestParameters &bestParam) {
                EXPECT_EQ(bestParam, Results[i]);
            }
    );
    for (; i < Lines.size(); ++i) {
        EvaluateBest(Lines[i], outputStrategyMock);
    }

}