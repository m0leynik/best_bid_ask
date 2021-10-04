#ifndef BEST_BID_ASK_LIB_NLOHMANN_JSON_PAYLOAD_PROCESSOR_H
#define BEST_BID_ASK_LIB_NLOHMANN_JSON_PAYLOAD_PROCESSOR_H

#include <memory>
#include <payload_processor_iface.h>

namespace bids_asks {
std::unique_ptr<IPayloadProcessor> CreateNlohmannJsonPayloadProcessor(std::shared_ptr<IPayloadProcessorCallback> payloadProcessorCallback);
} // bids_asks
#endif //BEST_BID_ASK_LIB_NLOHMANN_JSON_PAYLOAD_PROCESSOR_H