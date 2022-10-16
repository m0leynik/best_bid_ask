#ifndef BEST_BID_ASK_LIB_ORDER_H
#define BEST_BID_ASK_LIB_ORDER_H

#include <utility>

namespace bids_asks {
    // <price, amount>
    using order_t = std::pair<const double, double>;
} // bids_asks

#endif // BEST_BID_ASK_LIB_ORDER_H