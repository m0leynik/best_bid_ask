#ifndef BEST_BID_ASK_LIB_HELPERS_H
#define BEST_BID_ASK_LIB_HELPERS_H

#include <filesystem>
#include <fstream>
#include <map>
#include <string_view>
#include <unordered_map>

namespace helpers{
inline std::string ReadEntireFile(std::string_view inputFile)
{
    if (!std::filesystem::exists(inputFile)) {
        throw std::invalid_argument(std::string("File doesn't exist: ") + std::string(inputFile));
    }

    std::ifstream file(inputFile.data());

    file.seekg(0, std::ios::end);
    const size_t size = file.tellg();
    file.seekg(0);

    std::string contents(size, '\0');
    file.read(&contents[0], size);

    return contents;
}

// pair<price, amount>
template <typename T>
auto GetSmallestPricedOrder(const std::map<T, T> &container)
{
    return container.cbegin();
}

template <typename T>
auto GetGreatestPricedOrder(const std::map<T, T> &container)
{
    return container.crbegin();
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

template <typename container_t>
void PreallocateContainer(container_t &container, size_t size)
{
    constexpr bool HasReserve = requires(const container_t& container) {
        container.reserve(size);
    };

    if constexpr(HasReserve) {
        container.reserve(size);
    }
}
} // helpers
#endif //BEST_BID_ASK_LIB_HELPERS_H