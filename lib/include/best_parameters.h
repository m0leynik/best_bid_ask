#ifndef BEST_BID_ASK_LIB_BEST_PARAMETERS_H
#define BEST_BID_ASK_LIB_BEST_PARAMETERS_H

namespace bids_asks {
struct BestParameters
{
    double bestBidPrice {0.0};
    double bestBidAmount {0.0};
    double bestAskPrice {0.0};
    double bestAskAmount {0.0};
    uint64_t eventTime {};

    inline bool operator==(const bids_asks::BestParameters & other) const
    {
        return std::tie(this->bestBidAmount, this->bestAskAmount, this->bestAskPrice, this->bestBidPrice, this->eventTime) ==
               std::tie(other.bestBidAmount, other.bestAskAmount, other.bestAskPrice, other.bestBidPrice, other.eventTime);
    }
};
} // bids_asks

#endif //BEST_BID_ASK_LIB_BEST_PARAMETERS_H