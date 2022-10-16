#ifndef BEST_BID_ASK_LIB_ORDER_BOOK_IFACE_H
#define BEST_BID_ASK_LIB_ORDER_BOOK_IFACE_H

#include <best_parameters.h>
#include <order.h>

namespace bids_asks {
struct IOrderBook
{
    virtual BestParameters GetBestParameters() const = 0;
    virtual ~IOrderBook() {};
};

constexpr static size_t MaxOrders = 20u;

} // bids_asks
#endif //BEST_BID_ASK_LIB_ORDER_BOOK_IFACE_H