#include <stdexcept>

#include <rapidjson/document.h>

#include <rapidjson_payload_processor.h>
#include <payload_tags.h>

namespace bids_asks {
namespace {

void ThrowUnexpectedTypeError(const char *field, rapidjson::Type expected, rapidjson::Type actual)
{
    throw std::invalid_argument(std::string("Invalid type of field \"") + field + 
            "\". Expected type: " + std::to_string(expected) + 
            ", actual type: " + std::to_string(actual));
}

const auto GetBidsFromJson(const rapidjson::Document &document)
{
    const auto& bidsField = document[payload::tags::Bids];
    if (!bidsField.IsArray()) {
        ThrowUnexpectedTypeError(payload::tags::Bids, rapidjson::kArrayType, bidsField.GetType());
    }

    return bidsField.GetArray();
}

const auto GetAsksFromJson(const rapidjson::Document &document)
{
    const auto& asksField = document[payload::tags::Asks];
    
    if (!asksField.IsArray()) {
        ThrowUnexpectedTypeError(payload::tags::Asks, rapidjson::kArrayType, asksField.GetType());
    }

    return asksField.GetArray();
}

std::pair<double, double> GetPriceAndAmount(const rapidjson::Value &priceAmountArrayValue)
{
    if (!priceAmountArrayValue.IsArray()) {
        ThrowUnexpectedTypeError("price-amount-array", rapidjson::kArrayType, priceAmountArrayValue.GetType());
    }

    const auto & priceAmountArray = priceAmountArrayValue.GetArray();

    if (priceAmountArray.Size() != 2) {
        throw std::invalid_argument(
            std::string("Invalid size of price-amount array. Actual size is ") + std::to_string(priceAmountArray.Size()));
    }

    double price = priceAmountArray[0].Get<double>();
    double amount = priceAmountArray[1].Get<double>();

    return { price, amount };
}

void FillBidsFromUpdate(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // bids are sorted from high to low price
    const auto& bids = GetBidsFromJson(document);
    for (const auto &bid : bids) {
        payloadProcessorCallback.OnNewBidFromUpdate(GetPriceAndAmount(bid));
    }
}

void FillAsksFromUpdate(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // asks are sorted from low to high price
    const auto& asks = GetAsksFromJson(document);
    for (const auto &ask : asks) {
        payloadProcessorCallback.OnNewAskFromUpdate(GetPriceAndAmount(ask));
    }
}

void FillAsksFromSnapshot(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // asks are sorted from low to high price
    const auto& asks = GetAsksFromJson(document);
    for (const auto &ask : asks) {
        payloadProcessorCallback.OnNewAskFromSnapshot(GetPriceAndAmount(ask));
    }
}

void FillBidsFromSnapshot(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback)
{
    // bids are sorted from high to low price
    const auto& bids = GetBidsFromJson(document);
    for (const auto &bid : bids) {
        payloadProcessorCallback.OnNewBidFromSnapshot(GetPriceAndAmount(bid));
    }

}

uint64_t GetEventTime(const rapidjson::Document &document)
{
    const auto& eventTimeField = document[payload::tags::EventTime];
    if (!eventTimeField.IsUint64()) {
        ThrowUnexpectedTypeError(payload::tags::EventTime, rapidjson::kNumberType, eventTimeField.GetType());
    }

    return eventTimeField.GetUint64();
}

void ProcessSnapshot(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback)
{
    payloadProcessorCallback.OnNewSnapshot(GetEventTime(document));
    FillBidsFromSnapshot(document, payloadProcessorCallback);
    FillAsksFromSnapshot(document, payloadProcessorCallback);
}

void ProcessUpdate(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback)
{
    payloadProcessorCallback.OnNewUpdate(GetEventTime(document));
    FillBidsFromUpdate(document, payloadProcessorCallback);
    FillAsksFromUpdate(document, payloadProcessorCallback);
}

void ProcessEventsByType(const rapidjson::Document &document, IPayloadProcessorCallback &payloadProcessorCallback) 
{

    const auto& typeField = document[payload::tags::Type];
    if (!typeField.IsString()) {
        ThrowUnexpectedTypeError(payload::tags::Type, rapidjson::kStringType, typeField.GetType());
    }

    std::string_view type { typeField.GetString(), typeField.GetStringLength() };

    if (type == payload::tags::Snapshot) {
        ProcessSnapshot(document, payloadProcessorCallback);
    } else if (type == payload::tags::Update) {
        ProcessUpdate(document, payloadProcessorCallback);
    } else {
        throw std::invalid_argument(
                std::string("invalid \"") + payload::tags::Type +
                "\" tag value in payload: " + std::string(type));
    }
}

class RapidJsonPayloadProcessor final
        : public IPayloadProcessor
{
public:
    RapidJsonPayloadProcessor(std::shared_ptr<IPayloadProcessorCallback> payloadProcessorCallback)
        : m_payloadProcessorCallback(std::move(payloadProcessorCallback))
    {}

    // IPayloadProcessor
    void Process(std::string_view jsonPayload) override
    {
        rapidjson::Document document;
        document.Parse(jsonPayload.data(), jsonPayload.size());

        if (document.HasParseError()) {
            throw std::invalid_argument(
                    std::string("Rapidjson failed to parse document. Error code: ") +
                    std::to_string(document.GetParseError()) + ", error offset: " + std::to_string(document.GetErrorOffset())); 
        }

        ProcessEventsByType(document, *m_payloadProcessorCallback);
    }
private:
    std::shared_ptr<IPayloadProcessorCallback> m_payloadProcessorCallback{};
};
}

std::unique_ptr<IPayloadProcessor> CreateRapidJsonPayloadProcessor(std::shared_ptr<IPayloadProcessorCallback> payloadProcessorCallback)
{
    return std::make_unique<RapidJsonPayloadProcessor>(std::move(payloadProcessorCallback));
}
} // bids_asks