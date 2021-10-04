#ifndef BEST_BID_ASK_LIB_OUTPUT_STRATEGY_IFACE_H
#define BEST_BID_ASK_LIB_OUTPUT_STRATEGY_IFACE_H

#include <ctime>
#include <string>
#include <tuple>

#include "best_parameters.h"

namespace bids_asks {

struct IOutputStrategy
{
public:
    virtual void OnBestParametersEvaluated(const BestParameters &bestParams) = 0;
    virtual ~IOutputStrategy() {}
};
} // bids_asks


#endif //BEST_BID_ASK_LIB_OUTPUT_STRATEGY_IFACE_H