#include <fstream>
#include <iomanip>

#include "file_output_strategy.h"

namespace {
void OutputBestPriceAndAmountToFile(double price, double amount, std::ofstream &file)
{
    file << std::setprecision(2) << price << ", " << std::setprecision(6) << amount;
}

class FileOutputStrategy final :
        public bids_asks::IOutputStrategy
{
public:
    FileOutputStrategy(std::string_view outputFile) :
        m_file(outputFile.data(), std::ios_base::out | std::ios_base::trunc)
    {
        m_file << std::fixed;
    }

private:
    // bids_asks::IOutputStrategy
    void OnBestParametersEvaluated(const bids_asks::BestParameters &bestParams) override
    {
        // {event_time}, {best bid price}, {best bid amount}, {best ask price}, {best ask amount}
        m_file << bestParams.eventTime << ", ";
        OutputBestPriceAndAmountToFile(bestParams.bestBidPrice, bestParams.bestBidAmount, m_file);
        m_file << ", ";
        OutputBestPriceAndAmountToFile(bestParams.bestAskPrice, bestParams.bestAskAmount, m_file);
        m_file << std::endl;
    }
private:
    //std::wstring m_outputFile {};
    std::ofstream m_file;
};
} // namespace

namespace bids_asks {
std::unique_ptr<IOutputStrategy> CreateFileOutputStrategy(std::string_view outputFile)
{
    return std::make_unique<FileOutputStrategy>(outputFile);
}
} // bids_asks