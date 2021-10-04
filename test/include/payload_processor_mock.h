#ifndef INCLUDE_PAYLOAD_PROCESSOR_MOCK_H
#define INCLUDE_PAYLOAD_PROCESSOR_MOCK_H

#include <payload_processor_iface.h>

namespace bids_asks {

// The most simple and easy to understand solution is to pass
// bids, asks and timestamp as a single structure, but separate
// calls has been chosen to be more flexible in choosing order
// book structure and taking performance into account
class PayloadProcessorCallbackMock : public IPayloadProcessorCallback
{
public:
    MOCK_METHOD1(OnNewBidFromSnapshot, void(const order_t &));
    MOCK_METHOD1(OnNewAskFromSnapshot, void(const order_t &));
    MOCK_METHOD1(OnNewSnapshot, void(uint64_t));
    MOCK_METHOD1(OnNewBidFromUpdate, void(const order_t &));
    MOCK_METHOD1(OnNewAskFromUpdate, void(const order_t &));
    MOCK_METHOD1(OnNewUpdate, void(uint64_t));
};

class PayloadProcessorMock : public IPayloadProcessor
{
public:
    MOCK_METHOD2(Process, void(std::string_view, IPayloadProcessorCallback &));
};
} // bids_asks
#endif //INCLUDE_PAYLOAD_PROCESSOR_MOCK_H