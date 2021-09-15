#include <iostream>
#include "bids_asks.h"

int main()
{
    try {
        bids_asks::EvaluateBest("./huobi_global_depth.log", "./best_bids_and_asks.log");
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << std::string("Exception: ") + ex.what() << std::endl;
        return 1;
    }
}
