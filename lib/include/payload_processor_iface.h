#ifndef BEST_BID_ASK_LIB_PAYLOAD_PROCESSOR_IFACE_H
#define BEST_BID_ASK_LIB_PAYLOAD_PROCESSOR_IFACE_H

#include <utility>

namespace bids_asks {

// The most simple and easy to understand solution is to pass
// bids, asks and timestamp as a single structure, but separate
// calls has been chosen to be more flexible in choosing order
// book structure and taking performance into account
struct IPayloadProcessorCallback
{
    // <price, amount>
    using order_t = std::pair<double, double>;

    virtual void OnNewBidFromSnapshot(const order_t &bid) = 0;
    virtual void OnNewAskFromSnapshot(const order_t &ask) = 0;
    virtual void OnNewSnapshot(uint64_t timestamp) = 0;
    virtual void OnNewBidFromUpdate(const order_t &bid) = 0;
    virtual void OnNewAskFromUpdate(const order_t &ask) = 0;
    virtual void OnNewUpdate(uint64_t timestamp) = 0;

    virtual ~IPayloadProcessorCallback() {};
};

struct IPayloadProcessor
{
    virtual void Process(std::string_view payload) = 0;

    virtual ~IPayloadProcessor() {};
};
} // bids_asks
#endif //BEST_BID_ASK_LIB_PAYLOAD_PROCESSOR_IFACE_H