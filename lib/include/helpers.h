#ifndef BEST_BID_ASK_LIB_HELPERS_H
#define BEST_BID_ASK_LIB_HELPERS_H

#include <filesystem>
#include <fstream>
#include <string_view>

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
} // helpers
#endif //BEST_BID_ASK_LIB_HELPERS_H