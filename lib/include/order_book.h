#ifndef BEST_BID_ASK_LIB_ORDER_BOOK_H
#define BEST_BID_ASK_LIB_ORDER_BOOK_H

#include <concepts>
#include <iostream>

#include <order_book_iface.h>
#include <payload_processor_iface.h>

#include <helpers.h>
#include <orders_helpers.h>

namespace bids_asks {
namespace {
bool ShouldDeleteOrder(double amount)
{
    constexpr double AmountToDeleteOrder = 0.0;
    return (amount == AmountToDeleteOrder);
}

template<typename T>
concept map_of_orders_t = std::same_as<typename T::key_type, IPayloadProcessorCallback::order_t::first_type> 
                       && std::same_as<typename T::mapped_type, IPayloadProcessorCallback::order_t::second_type>;


template <map_of_orders_t map_t>
void UpdateOrder(const IPayloadProcessorCallback::order_t &order, map_t &orders)
{
    const auto& [price, amount] = order;

    if (ShouldDeleteOrder(amount)) {
        orders.erase(price);
        return;
    }
    // TODO: (?) use map::insert() parameter (hint) to optimize insertion
    orders[price] = amount;
}
}
template <map_of_orders_t map_t>
class OrderBookActualizer final
        : public IPayloadProcessorCallback
        , public IOrderBook
{
public:
    OrderBookActualizer()
    {
        helpers::PreallocateContainer(m_bids, MaxOrders);
        helpers::PreallocateContainer(m_asks, MaxOrders);
    }

    // IPayloadProcessorCallback
    void OnNewBidFromSnapshot(const order_t &bid) override
    {
        const auto& [price, amount] = bid;
        m_bids[price] = amount;
    }

    void OnNewAskFromSnapshot(const order_t &ask) override
    {
        const auto& [price, amount] = ask;
        m_asks[price] = amount;
    }

    void OnNewSnapshot(uint64_t timestamp) override
    {
        m_bids.clear();
        m_asks.clear();
        SetTimestamp(timestamp);
    }

    void OnNewBidFromUpdate(const order_t &bid) override
    {
        UpdateOrder(bid, m_bids);
    }

    void OnNewAskFromUpdate(const order_t &ask) override
    {
        UpdateOrder(ask, m_asks);
    }

    void OnNewUpdate(uint64_t timestamp) override
    {
        SetTimestamp(timestamp);
    }

    // IOrderBook
    BestParameters GetBestParameters() const override
    {
        if (m_bids.empty() || m_asks.empty()) {
            throw std::runtime_error("No orders found");
        }

        BestParameters bestParams {};
        bestParams.eventTime = m_timestamp;
        // TODO: (?) pass function as trait
        const auto bestAsk = helpers::GetSmallestPricedOrder(m_asks);
        bestParams.bestAskPrice = bestAsk->first;
        bestParams.bestAskAmount = bestAsk->second;

        const auto bestBid = helpers::GetGreatestPricedOrder(m_bids);
        bestParams.bestBidPrice = bestBid->first;
        bestParams.bestBidAmount = bestBid->second;

        return bestParams;
    }

private:
    void SetTimestamp(uint64_t timestamp)
    {
        m_timestamp = timestamp;
    }

    static void PrintOrders(const map_t & orders, std::ostream &outStream)
    {
        for (const order_t &order : orders)
        {
            outStream << "[" << order.first << ", " << order.second << "]" << std::endl;
        }
    }

    void Print(std::ostream &outStream) const
    {
        outStream << "Bids:" << std::endl;
        PrintOrders(m_bids, outStream);

        outStream << "Asks:" << std::endl;
        PrintOrders(m_asks, outStream);
    }

private:
    map_t m_bids {};
    map_t m_asks {};
    uint64_t m_timestamp {};
};

} // bids_asks
#endif //BEST_BID_ASK_LIB_ORDER_BOOK_H