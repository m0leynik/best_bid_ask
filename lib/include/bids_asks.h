#ifndef BEST_BID_ASK_LIB_BIDS_ASKS_H
#define BEST_BID_ASK_LIB_BIDS_ASKS_H

#include <string_view>

#include "output_strategy_iface.h"

namespace bids_asks {
void EvaluateBest(std::string_view inputFile, std::string_view outputFile);
void EvaluateBest(std::string_view fileContents, bids_asks::IOutputStrategy &outputStrategy);
} // bids_asks
#endif //BEST_BID_ASK_LIB_BIDS_ASKS_H
