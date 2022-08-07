#ifndef BEST_BID_ASK_LIB_ORDER_HELPERS_H
#define BEST_BID_ASK_LIB_ORDER_HELPERS_H

#include <boost/container/container_fwd.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/container/small_vector.hpp>

#include <map>
#include <unordered_map>

#include "order_book_iface.h"

namespace helpers{

template <typename container_t>
auto GetSmallestPricedOrderFromOrderedContainer(const container_t &container)
{
    return container.cbegin();
}

template <typename container_t>
auto GetGreatestPricedOrderFromOrderedContainer(const container_t &container)
{
    return container.crbegin();
}

template <typename T>
auto GetSmallestPricedOrder(const boost::container::small_flat_map<T, T, bids_asks::IOrderBook::MaxOrders> &container)
{
    return GetSmallestPricedOrderFromOrderedContainer(container);
}

template <typename T>
auto GetGreatestPricedOrder(const boost::container::small_flat_map<T, T, bids_asks::IOrderBook::MaxOrders> &container)
{
    return GetGreatestPricedOrderFromOrderedContainer(container);
}

template <typename T>
auto GetSmallestPricedOrder(const boost::container::flat_map<T, T> &container)
{
    return GetSmallestPricedOrderFromOrderedContainer(container);
}

template <typename T>
auto GetGreatestPricedOrder(const boost::container::flat_map<T, T> &container)
{
    return GetGreatestPricedOrderFromOrderedContainer(container);
}

template <typename T>
auto GetSmallestPricedOrder(const std::map<T, T> &container)
{
    return GetSmallestPricedOrderFromOrderedContainer(container);
}

template <typename T>
auto GetGreatestPricedOrder(const std::map<T, T> &container)
{
    return GetGreatestPricedOrderFromOrderedContainer(container);
}

template <typename T>
auto GetSmallestPricedOrder(const std::unordered_map<T, T> &container)
{
    return std::min_element(container.cbegin(), container.cend(),
                            [](const auto& l, const auto& r) { return l.first < r.first; });
}

template <typename T>
auto GetGreatestPricedOrder(const std::unordered_map<T, T> &container)
{
    return std::max_element(container.cbegin(), container.cend(),
                            [](const auto& l, const auto& r) { return l.first < r.first; });
}
} // helpers
#endif //BEST_BID_ASK_LIB_ORDER_HELPERS_H