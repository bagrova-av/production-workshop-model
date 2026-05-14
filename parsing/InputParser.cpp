#include "InputParser.h"
#include "validationUtils.h"
#include "constants.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool InputParser::isLineEmpty(std::stringstream& ss)
{
    std::string extra;
    return !(ss >> extra);
}

void InputParser::exitWithError(const std::string& lineWithError)
{
    std::cout << lineWithError << '\n';
    exit(0);
}

Config InputParser::parseFile(const std::string& fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        exit(0);
    }

    std::string line;
    Config config;

    if (!std::getline(file, line))
    {
        exit(0);
    }
    std::stringstream ssWithMN(line);
    if (!(ssWithMN >> config.countMachines >> config.countProductsTypes) ||
        !isLineEmpty(ssWithMN))
    {
        exitWithError(line);
    }
    if (!validateCountMachines(config.countMachines) ||
        !validateCountProductsTypes(config.countProductsTypes))
    {
        exitWithError(line);
    }

    for (int i = 0; i < config.countMachines - 1; ++i)
    {
        if (!std::getline(file, line))
        {
            exitWithError(line);
        }
        std::stringstream ssTimes(line);
        for (int j = 0; j < config.countProductsTypes; ++j)
        {
            int time;
            if (!(ssTimes >> time))
            {
                exitWithError(line);
            }
            if (!validateOperationTime(time))
            {
                exitWithError(line);
            }
            config.operationsTimes.push_back(time);
        }
        if (!isLineEmpty(ssTimes))
        {
            exitWithError(line);
        }
    }

    long long totalProducts = 0;
    for (int j = 0; j < config.countProductsTypes; ++j)
    {
        if (!std::getline(file, line))
        {
            exitWithError(line);
        }
        std::stringstream ssQueue(line);
        int queueSize;
        if (!(ssQueue >> queueSize))
        {
            exitWithError(line);
        }
        if (queueSize < 0)
        {
            exitWithError(line);
        }
        
        std::vector<int> queue;
        for (int counter = 0; counter < queueSize; ++counter)
        {
            int productType;
            if (!(ssQueue >> productType))
            {
                exitWithError(line);
            }
            if (!validateProductType(productType, config))
            {
                exitWithError(line);
            }
            queue.push_back(productType);
        }
        if (!isLineEmpty(ssQueue))
        {
            exitWithError(line);
        }
        config.initialQueues.push_back(queue);
        totalProducts += queueSize;
    }
    if (!validateTotalCountProducts(totalProducts))
    {
        std::cout << "The total number of products in the workshop exceeds the maximum allowed number"
                  << std::to_string(MAX_COUNT_PRODUCTS_IN_WORKSHOP) << '\n'
                  << "The last line in the input file is: " << '\n';
        exitWithError(line);
    }

    return config;
}