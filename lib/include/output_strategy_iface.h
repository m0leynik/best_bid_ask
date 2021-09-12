#ifndef BEST_BID_ASK_LIB_OUTPUT_STRATEGY_IFACE_H
#define BEST_BID_ASK_LIB_OUTPUT_STRATEGY_IFACE_H

#include <ctime>
#include <string>

namespace bids_asks {
struct BestParameters
{
    double bestBidPrice {0.0};
    double bestBidAmount {0.0};
    double bestAskPrice {0.0};
    double bestAskAmount {0.0};
    uint64_t eventTime;
};

struct IOutputStrategy
{
public:
    virtual void OnBestParametersEvaluated(const BestParameters &bestParams) = 0;
    virtual ~IOutputStrategy() {}
};
} // bids_asks

#endif //BEST_BID_ASK_LIB_OUTPUT_STRATEGY_IFACE_H