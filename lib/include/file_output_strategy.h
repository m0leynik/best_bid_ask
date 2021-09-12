#ifndef BEST_BID_ASK_LIB_FILE_OUTPUT_STRATEGY_H
#define BEST_BID_ASK_LIB_FILE_OUTPUT_STRATEGY_H

#include <memory>
#include <string_view>

#include "output_strategy_iface.h"

namespace bids_asks {
//std::unique_ptr<IOutputStrategy> CreateSimpleConsoleOutputStrategy();
std::unique_ptr<IOutputStrategy> CreateFileOutputStrategy(std::string_view outputFile);
}
#endif //BEST_BID_ASK_LIB_FILE_OUTPUT_STRATEGY_H
