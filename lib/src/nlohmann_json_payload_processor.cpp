#include <stdexcept>

#include <nlohmann/json.hpp>

#include <nlohmann_json_payload_processor.h>
#include <payload_tags.h>

namespace bids_asks {
namespace {

const auto & GetBidsFromJson(const nlohmann::json &j)
{
    return j[payload::tags::Bids].get_ref<const nlohmann::json::array_t &>();
}

const auto & GetAsksFromJson(const nlohmann::json &j)
{
    return j[payload::tags::Asks].get_ref<const nlohmann::json::array_t &>();
}

std::pair<double, double> GetPriceAndAmount(const nlohmann::json &j)
{
    const auto & array = j.get_ref<const nlohmann::json::array_t &>();
    if (array.size() != 2) {
        throw std::invalid_argument(std::string("invalid size of json array: ") + to_string(j));
    }

    double price = array[0].get<nlohmann::json::number_float_t>();
    double amount = array[1].get<nlohmann::json::number_float_t>();

    return { price, amount };
}

void FillBidsFromUpdate(const nlohmann::json &j, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // bids are sorted from high to low price
    const auto& bids = GetBidsFromJson(j);
    for (const auto &bid : bids) {
        payloadProcessorCallback.OnNewBidFromUpdate(GetPriceAndAmount(bid));
    }
}

void FillAsksFromUpdate(const nlohmann::json &j, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // asks are sorted from low to high price
    const auto& asks = GetAsksFromJson(j);
    for (const auto &ask : asks) {
        payloadProcessorCallback.OnNewAskFromUpdate(GetPriceAndAmount(ask));
    }
}

void FillAsksFromSnapshot(const nlohmann::json &j, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // asks are sorted from low to high price
    const auto& asks = GetAsksFromJson(j);
    for (const auto &ask : asks) {
        payloadProcessorCallback.OnNewAskFromSnapshot(GetPriceAndAmount(ask));
    }
}

void FillBidsFromSnapshot(const nlohmann::json &j, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // bids are sorted from high to low price
    const auto& bids = GetBidsFromJson(j);
    for (const auto &bid : bids) {
        payloadProcessorCallback.OnNewBidFromSnapshot(GetPriceAndAmount(bid));
    }

}

void ProcessSnapshot(const nlohmann::json &j, IPayloadProcessorCallback &payloadProcessorCallback)
{
    payloadProcessorCallback.OnNewSnapshot(j[payload::tags::EventTime]);
    FillBidsFromSnapshot(j, payloadProcessorCallback);
    FillAsksFromSnapshot(j, payloadProcessorCallback);
}

void ProcessUpdate(const nlohmann::json &j, IPayloadProcessorCallback &payloadProcessorCallback)
{
    payloadProcessorCallback.OnNewUpdate(j[payload::tags::EventTime]);
    FillBidsFromUpdate(j, payloadProcessorCallback);
    FillAsksFromUpdate(j, payloadProcessorCallback);
}

// TODO: (?) replace interface IPayloadProcessor with a single function bids_asks::nlohmann_json::ProcessPayload(...)
class NlohmannJsonPayloadProcessor final
        : public IPayloadProcessor
{
public:
    NlohmannJsonPayloadProcessor(std::shared_ptr<IPayloadProcessorCallback> payloadProcessorCallback)
        : m_payloadProcessorCallback(std::move(payloadProcessorCallback))
    {}

    // IPayloadProcessor
    void Process(std::string_view jsonPayload) override
    {
        const nlohmann::json j = nlohmann::json::parse(jsonPayload);
        const auto &type = j[payload::tags::Type].get_ref<const nlohmann::json::string_t&>();

        if (type == payload::tags::Snapshot) {
            ProcessSnapshot(j, *m_payloadProcessorCallback);
        } else if (type == payload::tags::Update) {
            ProcessUpdate(j, *m_payloadProcessorCallback);
        } else {
            throw std::invalid_argument(
                    std::string("invalid \"") + payload::tags::Type +
                    "\" tag value in payload: " + to_string(j));
        }
    }
private:
    std::shared_ptr<IPayloadProcessorCallback> m_payloadProcessorCallback{};
};
}

std::unique_ptr<IPayloadProcessor> CreateNlohmannJsonPayloadProcessor(std::shared_ptr<IPayloadProcessorCallback> payloadProcessorCallback)
{
    return std::make_unique<NlohmannJsonPayloadProcessor>(std::move(payloadProcessorCallback));
}
} // bids_asks