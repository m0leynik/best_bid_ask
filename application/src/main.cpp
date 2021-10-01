#include <iostream>

#include <file_output_strategy.h>
#include <helpers.h>

#include "bids_asks.h"


void EvaluateBest(std::string_view inputFile, std::string_view outputFile)
{
    const std::string fileContents = helpers::ReadEntireFile(inputFile);
    const auto outputStrategy = bids_asks::CreateFileOutputStrategy(outputFile);

    bids_asks::EvaluateBest(fileContents, *outputStrategy);
}

int main()
{
    try {
        EvaluateBest("./huobi_global_depth.log", "./best_bids_and_asks.log");
        return 0;
    } catch (const std::exception &ex) {
        std::cerr << std::string("Exception: ") + ex.what() << std::endl;
        return 1;
    }
}
