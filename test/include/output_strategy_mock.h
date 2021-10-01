#ifndef INCLUDE_OUTPUT_STRATEGY_MOCK_H
#define INCLUDE_OUTPUT_STRATEGY_MOCK_H

#include <output_strategy_iface.h>
#include <gmock/gmock.h>

namespace bids_asks {
class OutputStrategyMock : public IOutputStrategy
{
public:
    MOCK_METHOD1(OnBestParametersEvaluated, void(const BestParameters &));
};
} // bids_asks

#endif //INCLUDE_OUTPUT_STRATEGY_MOCK_H