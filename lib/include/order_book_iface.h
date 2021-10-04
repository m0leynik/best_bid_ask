#ifndef BEST_BID_ASK_LIB_ORDER_BOOK_IFACE_H
#define BEST_BID_ASK_LIB_ORDER_BOOK_IFACE_H

#include <best_parameters.h>

namespace bids_asks {
struct IOrderBook
{
    virtual BestParameters GetBestParameters() const = 0;

    virtual ~IOrderBook() {};
};

} // bids_asks
#endif //BEST_BID_ASK_LIB_ORDER_BOOK_IFACE_H